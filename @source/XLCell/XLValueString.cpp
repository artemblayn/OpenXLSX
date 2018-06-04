//
// Created by KBA012 on 18-12-2017.
//

#include "XLValueString.h"
#include "../XLWorkbook/XLException.h"
#include "XLCell.h"
#include "XLCellValue.h"

using namespace OpenXLSX;
using namespace std;

/**
 * @details This constructor initializes the m_type variable depending on whether shared strings are available or not.
 * @todo What if SharedStrings are enabled, but this particular string has an ordinary string?
 * @todo Find a way to initialize properly.
 */
XLValueString::XLValueString()
    : XLValue(),
      m_type(XLStringType::String)
{
}

/**
 * @details This function sets the cell node in the underlying XML file to the right value, based on the input. A value
 * node and a type attribute is created (if they don't exists already) and set to the appropriate values. If
 * shared strings are enabled, the appropriate string will be looked up or appended to the shared string repository.
 * Otherwise, an ordinary string will be created.
 * @todo InlineStrings are not implemented.
 */
 /*
void XLValueString::Initialize(string_view stringValue)
{
    if (!ParentCellValue()->HasValueNode()) ParentCellValue()->CreateValueNode();
    if (!ParentCellValue()->HasTypeAttribute()) ParentCellValue()->CreateTypeAttribute();

    ParentCellValue()->ParentCell()->SetModified();

    switch (m_type) {
        case XLStringType::InlineString:
            // Fall through
        case XLStringType::SharedString:
            if (!ParentCellValue()->ParentCell()->ParentWorkbook()->SharedStrings()->StringExists(stringValue)) {
                ParentCellValue()->ParentCell()->ParentWorkbook()->SharedStrings()->AppendString(stringValue);
            }
            ParentCellValue()->SetValueNode(to_string(ParentCellValue()->ParentCell()->ParentWorkbook()->SharedStrings()->GetStringIndex(stringValue)));
            ParentCellValue()->SetTypeAttribute(TypeString());
            break;

        case XLStringType::String:
            ParentCellValue()->SetValueNode(stringValue);
            ParentCellValue()->SetTypeAttribute(TypeString());
            break;
    }
} */

/**
 * @details The Set method sets the object to the given type and with the given string value.
 */
const std::string & XLValueString::Set(string_view stringValue, XLStringType type)
{
    m_type = type;
    Initialize(stringValue);
}

/**
 * @details Depending on the type of string, this function will either return the value of the value node or return
 * the shared string with the right index.
 * @todo InlineString is not implemented.
 */
const std::string &XLValueString::Get() const
{
    switch (m_type) {
        case XLStringType::String:
            m_cache = ParentCellValue()->ValueNode().text().get();
            return m_cache;

        case XLStringType::SharedString:
            m_cache = ParentCellValue()->ParentCell()->ParentWorkbook()->SharedStrings()->GetStringNode(stoul(ParentCellValue()->ValueNode().text().get())).text().get();
            return m_cache;

        case XLStringType::InlineString:
            throw XLException("Inline String not implemented");
    }

    return m_cache; // Included to silence compiler warning.
}

/**
 * @details Returns an XLStringType object, representing the type of the current string.
 */
XLStringType XLValueString::StringType() const
{
    return m_type;
}

/**
 * @details Returns XLCellType::String.
 */
XLValueType XLValueString::ValueType() const
{
    return XLValueType::String;
}

/**
 * @details Returns XLCellType::String.
 */
XLCellType XLValueString::CellType() const
{
    return XLCellType::String;
}

/**
 * @details Returns a std::string with the type string corresponding to the cell type attribute in the XML file.
 */
std::string XLValueString::TypeString() const
{
    switch (m_type) {
        case XLStringType::String:
            return "str";

        case XLStringType::SharedString:
            return "s";

        case XLStringType::InlineString:
            return "inlineStr";
    }

    return ""; // To silence compiler warning
}

/**
 * @details This method checks if the type of string is indeed SharedString and returns the string index, if it is.
 * Otherwise, an XLException is thrown.
 */
long XLValueString::SharedStringIndex() const
{
    if (m_type == XLStringType::SharedString)
        return ParentCellValue()->ParentCell()->ParentWorkbook()->SharedStrings()->GetStringIndex(String());
    else
        throw XLException("String is not of type SharedString");
}

/**
 * @details This function returns the value of the string. Internally, it simply calls the String method.
 */
string XLValueString::AsString() const
{
    return String();
}


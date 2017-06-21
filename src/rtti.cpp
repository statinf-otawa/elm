/*
 *	rtti module implementation
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2017, IRIT UPS.
 *
 *	OTAWA is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	OTAWA is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with OTAWA; if not, write to the Free Software
 *	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <elm/data/HashMap.h>
#include <elm/rtti.h>
#include <elm/serial2/serial.h>

namespace elm {

/**
 * @fn const Type& elm::type_of<T>();
 * Get the instance type corresponding to the given type.
 * Retrieve automatically the type for scalar and pointer
 * or for any specialization of type_of(). For class types,
 * look for the static `__type` member.
 * @param T		Type an instance is looked for.
 * @return		Corresponding type instance.
 * @ingroup rtti
 */

namespace rtti {

/**
 * @defgroup rtti  Run-Time Type Information
 *
 * This module provides information for type at run-time.
 * It may be used for very different purposes as:
 * * automatic serialization / deserialization,
 * * automatic interface generation,
 * * automatic display of instances,
 * * run-time typed and generic storage.
 *
 * For each type, it can provide a @ref Type descriptor that
 * includes:
 * * name as a string,
 * * parent for classes,
 * * casting information,
 * * list of attributes,
 * * list of methods and constructors.
 *
 * This module provides several specializations for the different
 * possible types:
 * * Enum for enumeration type,
 * * Class for classes.
 *
 * Additionally, you can use the @ref type_of<T>() function to get
 * the RTTI type instance representing a type.
 *
 * RTTI for a Class
 * ================
 *
 * Basically, it is as easy as declaring a global variable for the class
 * and linking it to the @ref type_of<T>() function:
 *
 * @code
 * // in the header
 * #include <elm/rtti.h>
 *
 * class MyClass {
 * 	...
 * };
 *
 * extern Class<MyClass> MyClass_type;
 * namespace elm { template <> const Type& type<MyClass>(void) { return MyClass_type; } }
 *
 * // in the source
 * Class<MyClass> MyClass_type("MyClass");
 * @endcode
 *
 * To avoid the ugly line, you can declare type instance as a static member of the class
 * (the @ref elm::type_of() function will find it automatically):
 * @code
 * // in the header
 * #include <elm/rtti.h>
 *
 * class MyClass {
 * public:
 * 	static Class<MyClass> __type;
 * 	...
 * };
 *
 * // in the source
 * Class<MyClass> MyClass::__type("MyClass");
 * @endcode
 *
 * Another possibility is to use the integrated macros of RTTI system:
 * @code
 * // in the header
 * #include <elm/rtti.h>
 *
 * CLASS(MyClass)
 * 	...
 * END_CLASS
 *
 * // in the source
 * IMPLEMENT(MyClass)
 * @endcode
 *
 * If your class is extending another RTTI class, you will use instead:
 * @code
 * // in the header
 * #include <elm/rtti.h>
 *
 * CLASS_EXTEND(MyClass, MyBaseClass)
 * 	...
 * END_CLASS
 *
 * // in the source
 * IMPLEMENT(MyClass)
 * @endcode
 * And that's it.
 *
 * RTTI for Enumeration
 * ====================
 *
 * Providing RTTI for enumration provides several benefits:
 * * for automatic serialization / unserialization function,
 * * for automatic output or input function,
 * * provide aliases to some enumeration values.
 *
 * In the header file, you have to declare your enumeration
 * following by a call to macro @ref DECLARE_ENUM:
 * @code
 * #include <elm/rtti.h>
 *
 * typedef enum my_enum_t {
 * 	A = 0,
 * 	B = 1,
 * 	C = 2
 * } my_enum_t;
 * DECLARE_ENUM(my_enum_t);
 * @endcode
 *
 * Then you have to create in the source the instance of type
 * representing the enumeration followed by a call to macro
 * @ref DEFINE_ENUM:
 * @code
 * Enum my_enum_t_type(Enum::make("my_enum_t")
 * 	.value("A", A)
 * 	.value("B", B)
 * 	.value("C", C)
 * 	.alias("AA", A));
 * DEFINE_ENUM(my_enum_t, my_enum_t_type);
 * @endcode
 */

/**
 * Map of available types.
 */
static HashMap<string, const Type *> type_map;

/**
 */
class PointerType: public Type {
public:
	PointerType(const Type *t): Type(t->name() + "*") { }
	virtual bool isPointer(void) const { return true; }
};

/**
 * @class Enumerable
 * This interface is provided by type supporting enumerated data.
 * @ingroup rtti
 */

/**
 */
Enumerable::~Enumerable(void) {
}

/**
 * @fn const Type& Enumerable::type(void) const;
 * Get the type corresponding to this enumerable interface.
 * @return	Corresponding type.
 */

/**
 * @fn int Enumerable::valueFor(cstring text) const;
 * @param text	Text of the value for.
 * @return		Found value or -1 if not found.
 */

/**
 * @fn cstring Enumerable::nameFor(int value) const;
 * Find the name corresponding to the gi en enumerated
 * value.
 * @param value	Value to look text for.
 * @return		If found, corresponding name or empty string.
 */


/**
 * @class Serializable
 * This interface is provided by types supporting serializable
 * facilities.
 * @ingroup rtti
 */

/**
 */
Serializable::~Serializable(void) {
}

/**
 * @fn const Type& Serializable::type(void) const;
 * Get the type corresponding to this enumerable interface.
 * @return	Corresponding type.
 */

/**
 * @fn void Serializable::serialize(serial2::Serializer& ser, const void *data);
 * Serialize the data passed in parameter that must be of type corresponding
 * to the interface.
 * @param ser	Serializer to use.
 * @param data	Pointer to data to serialize.
 */

/**
 * @fn void Serializable::unserialize(serial2::Unserializer& uns, void *data);
 * Unserialize the coming data from the unserializer to the place pointed by data.
 * This place must be of the same type as the serializable type.
 * @param uns	Unserializer to get data from.
 * @param data	Pointer to place to unserialize in.
 */


/**
 * @class Type
 * Materialize the concept of type. Used for serialization or
 * with classes like variant.
 * @ingroup rtti
 */

/**
 * Initializer to avoid the initialization issue of C++.
 */
Initializer<Type> Type::_init(false);

/**
 */
Type::Type(string name): _name(name) {
	_init.record(this);
}

/**
 * For internal use only. Do not call it.
 */
void Type::initialize(void) {
	type_map.put(name(), this);
}

/**
 * Get a type by its name.
 * @param name	Name of looked type.
 * @return		Found type or null.
 */
const Type *Type::get(string name) {
	_init.startup();
	return type_map.get(name, 0);
}

/**
 */
Type::~Type(void) { }

/**
 * Obtain pointer type on the current type.
 * @return	Corresponding pointer type.
 */
const Type *Type::pointer(void) const {
	if(!_pointer)
		_pointer = new PointerType(this);
	return _pointer;
}

/**
 * @fn string Type::name(void) const;
 * Get the name of the type.
 */

/**
 * Test if the type can be casted to the given type.
 * @param t		Type to cast to.
 * @return		True if the type can be casted to the given type, false else.
 */
bool Type::canCast(const Type *t) const {
	return t == this;
}

/**
 * Test if the type is void.
 * Default implementation returns false.
 * @return	True if the type is void, false else.
 */
bool Type::isVoid(void) const {
	return false;
}

/**
 * Test if the type is a boolean.
 * @return	True if the type is boolean, false else.
 */
bool Type::isBool(void) const {
	return false;
}

/**
 * Test if the type is a integer.
 * @return	True if the type is boolean, false else.
 */
bool Type::isInt(void) const {
	return false;
}

/**
 * Test if the type is a floating-point real.
 * @return	True if the type is boolean, false else.
 */
bool Type::isFloat(void) const {
	return false;
}

/**
 * Test if the type is a pointer.
 * @return	True if the type is boolean, false else.
 */
bool Type::isPtr(void) const {
	return false;
}

/**
 * Test if the type is a class. Default implementation
 * return false.
 * @return	True if the type is a class, false else.
 */
bool Type::isClass(void) const {
	return false;
}

/**
 * Get the class description corresponding to this type.
 * If the type is not a class, an assertion failure is raised.
 * @return	Corresponding class instance.
 */
const AbstractClass& Type::asClass(void) const {
	ASSERTP(false, "type " << name() << " is not a class!");
	return *null<AbstractClass>();
}

/**
 * Test if the type is enumerated. Default implementation
 * return false.
 * @return	True if the type is a class, false else.
 */
bool Type::isEnum(void) const {
	return false;
}

/**
 * Get the enumeration description corresponding to this type.
 * If the type is not enumerated, an assertion failure is raised.
 * @return	Corresponding class instance.
 */
const Enumerable& Type::asEnum(void) const {
	ASSERTP(false, "type " << name() << " is not enumerated!");
	return *null<Enumerable>();
}

/**
 * Test if the type is serializable. Default implementation
 * return false.
 * @return	True if the type is serializable, false else.
 */
bool Type::isSerial(void) const {
	return false;
}

/**
 * Return the serializable instance of this type.
 * @warning Will raise assertion failure if the type
 * is not serializable.
 * @return	Serializable instance.
 */
const Serializable& Type::asSerial(void) const {
	ASSERTP(false, "type " << name() << " is not serializable!");
	return *null<Serializable>();
}


/**
 */
template <class T>
class IntType: public Type, public Serializable {
public:
	IntType(void): Type(type_info<T>::name()) { }
	virtual bool isInt(void) const { return true; }
	virtual bool canCast(const Type *t) const { return t->isBool() || t->isInt() || t->isFloat(); }
	virtual bool isSerial(void) const { return true; }
	virtual const Serializable& asSerial(void) const { return *this; }

	// Serializable interface
	virtual const Type& type(void) const { return *this; }
	virtual void *instantiate(void) const { return new T; }
	virtual void serialize(serial2::Serializer& ser, const void *data) const { ser.onValue(*static_cast<const T *>(data)); }
	virtual void unserialize(serial2::Unserializer& uns, void *data) const { uns.onValue(*static_cast<T *>(data)); }
};

/**
 * Type for signed 8-bit integer.
 * @ingroup rtti
 */
const Type& int8_type = Single<IntType<t::int8> >::_;

/**
 * Type for unsigned 8-bit integer.
 * @ingroup rtti
 */
const Type& uint8_type = Single<IntType<t::uint8> >::_;

/**
 * Type for signed 16-bit integer.
 * @ingroup rtti
 */
const Type& int16_type = Single<IntType<t::int16> >::_;

/**
 * Type for unsigned 8-bit integer.
 * @ingroup rtti
 */
const Type& uint16_type = Single<IntType<t::uint16> >::_;

/**
 * Type for signed 32-bit integer.
 * @ingroup rtti
 */
const Type& int32_type = Single<IntType<t::int32> >::_;

/**
 * Type for unsigned 32-bit integer.
 * @ingroup rtti
 */
const Type& uint32_type = Single<IntType<t::uint32> >::_;

/**
 * Type for signed 64-bit integer.
 * @ingroup rtti
 */
const Type& int64_type = Single<IntType<t::int64> >::_;

/**
 * Type for unsigned 64-bit integer.
 * @ingroup rtti
 */
const Type& uint64_type = Single<IntType<t::uint64> >::_;


/**
 * Type class for floating-point reals.
 */
template <class T>
class FloatType: public Type, public Serializable {
public:
	FloatType(void): Type(type_info<T>::name()) { }
	virtual bool isFloat(void) const { return true; }
	virtual bool canCast(const Type *t) const { return t->isBool() || t->isInt() || t->isFloat(); }
	virtual bool isSerial(void) const { return true; }
	virtual const Serializable& asSerial(void) const { return *this; }

	// Serializable interface
	virtual const Type& type(void) const { return *this; }
	virtual void *instantiate(void) const { return new T; }
	virtual void serialize(serial2::Serializer& ser, const void *data) const { ser.onValue(*static_cast<const T *>(data)); }
	virtual void unserialize(serial2::Unserializer& uns, void *data) const { uns.onValue(*static_cast<T *>(data)); }
};

/**
 * Type for single-precision floating-point real.
 * @ingroup rtti
 */
const Type& float_type = Single<FloatType<float> >::_;

/**
 * Type for double-precision floating-point real.
 * @ingroup rtti
 */
const Type& double_type = Single<FloatType<double> >::_;

/**
 * Type for quadruple-precision floating-point real.
 * @ingroup rtti
 */
const Type& long_double_type = Single<FloatType<long double> >::_;

/**
 */
class BoolType: public Type, public Serializable {
public:
	BoolType(void): Type("boo") { }
	virtual bool canCast(const Type *t) const { return t == this || t->isInt(); }
	virtual bool isBool(void) const { return true; }
	virtual bool isSerial(void) const { return true; }
	virtual const Serializable& asSerial(void) const { return *this; }

	// Serializable interface
	virtual const Type& type(void) const { return *this; }
	virtual void *instantiate(void) const { return new bool(false); }
	virtual void serialize(serial2::Serializer& ser, const void *data) const { ser.onValue(*static_cast<const bool *>(data)); }
	virtual void unserialize(serial2::Unserializer& uns, void *data) const { uns.onValue(*static_cast<bool *>(data)); }
};

/**
 * Type for booleans.
 * @ingroup rtti
 */
const Type& bool_type = Single<BoolType>::_;

/**
 */
class StringType: public Type, public Serializable {
public:
	StringType(void): Type("string") { }
	virtual bool isSerial(void) const { return true; }
	virtual const Serializable& asSerial(void) const { return *this; }

	// Serializable interface
	virtual const Type& type(void) const { return *this; }
	virtual void *instantiate(void) const { return new string; }
	virtual void serialize(serial2::Serializer& ser, const void *data) const { ser.onValue(*static_cast<const string *>(data)); }
	virtual void unserialize(serial2::Unserializer& uns, void *data) const { uns.onValue(*static_cast<string *>(data)); }
};


/**
 * Type for strings.
 * @ingroup rtti
 */
const Type& string_type = Single<StringType>::_;

/**
 * Class to representd void type.
 * @ingroup rtti
 */
class VoidType: public AbstractClass {
public:
	VoidType(void): AbstractClass("void",  static_cast<const AbstractClass &>(void_type)) { }
	virtual bool isVoid(void) const { return true; }
	virtual void *instantiate(void) const { ASSERTP(false, "void type cannot be instantiated!"); return 0; }
};

/**
 * Void type representation.
 * @ingroup rtti
 */
const Type& void_type = Single<VoidType>::_;


/**
 */
class CStringType: public Type, public Serializable {
public:
	CStringType(void): Type("cstring") { }
	virtual bool isSerial(void) const { return true; }
	virtual const Serializable& asSerial(void) const { return *this; }

	// Serializable interface
	virtual const Type& type(void) const { return *this; }
	virtual void *instantiate(void) const { return new cstring; }
	virtual void serialize(serial2::Serializer& ser, const void *data) const { ser.onValue(*static_cast<const cstring *>(data)); }
	virtual void unserialize(serial2::Unserializer& uns, void *data) const { uns.onValue(*static_cast<cstring *>(data)); }
};

/**
 * Type for C strings.
 * @ingroup rtti
 */
const Type& cstring_type = Single<CStringType>::_;


/**
 * @class AbstractClass
 *
 * Abstract class to represent classes in RTTI. It is mainly used
 * for serialization / deserialization.
 *
 * @ingroup rtti
 */

/**
 * Build the abstract class.
 * @param name	Class named (fully qualified).
 * @param base	Base class (default none).
 */
AbstractClass::AbstractClass(CString name, const AbstractClass& base): rtti::Type(name), _base(base) {
}

/**
 * @fn AbstractClass *AbstractClass::base(void) const;
 * Get the parent class.
 * @return	Parent class.
 */

/**
 * @fn void *AbstractClass::instantiate(void);
 * Call this function to get a new allocated fresh object of the corresponding class.
 * This function must be overridden to perform the actual allocation
 * and the class must support constructor without any argument.
 * @return	Allocated instance of class.
 */

/**
 * Test if the current class is base class of the given one.
 * Notice that this class is a base class of clazz even if this
 * class equals the class clazz.
 * @param clazz		Class to test.
 * @return			True if this class is a base class, false else.
 */
bool AbstractClass::baseOf(const AbstractClass *clazz) {
	while(clazz) {
		if(clazz == this)
			return true;
		clazz = &clazz->base();
	}
	return false;
}

/**
 */
bool AbstractClass::isClass(void) const {
	return true;
}

/**
 */
const AbstractClass& AbstractClass::asClass(void) const {
	return *this;
}

/**
 * Cast a pointer from the current to the base type
 * of this type. Default implementation is the identity.
 * @param ptr	Pointer to the current type.
 * @return		Pointer to the base type.
 */
void *AbstractClass::upCast(void *ptr) const {
	return ptr;
}

/**
 * Cast a pointer from the base type to the current type.
 * Default implementation is the identity.
 * @param ptr	Pointer to the base type.
 * @return		Pointer to the current type.
 */
void *AbstractClass::downCast(void *ptr) const {
	return ptr;
}

/**
 * Convert ptr, pointer to the current type, to
 * a pointer of type cls. Raise an assertion
 * failure if cls is not a base class of the current class.
 * @param ptr	Current class pointer.
 * @param cls	Base class of the current class, to cast to.
 * @return	cls class pointer.
 */
void *AbstractClass::upCast(void *ptr, const AbstractClass& cls) const {
	const AbstractClass *cur = this;
	while(cur != &cls) {
		ASSERT(cur != &void_type);
		ptr = cur->upCast(ptr);
		cur = &cur->base();
	}
	return ptr;
}


/**
 * Down cast a pointer in the current type to a pointer in cls type.
 * If the current class is not a base class of the class cls,
 * raises an assertion failure.
 * @param ptr	Pointer to the current type.
 * @param cls	Child class of the current class, to cast to.
 * @return		Pointer to class cls.
 */
void *AbstractClass::downCast(void *ptr, const AbstractClass& cls) const {

	// build back the list if bases
	Vector<const AbstractClass *> bases;
	const AbstractClass *cur = &cls;
	while(cur != this) {
		bases.add(cur);
		ASSERT(cur != &void_type);
		cur = &cur->base();
	}

	// cast in reverse order
	for(int i = bases.count() - 1; i >= 0; i--)
		ptr = bases[i]->downCast(ptr);
	return ptr;
}


/**
 * @class ObjectType
 * Base class of RTTI objects.
 * @ingroup rtti
 */

/**
 */
/*ObjectType::ObjectType(void): AbstractClass("elm::Object", static_cast<const AbstractClass &>(object_type)) {
}*/

/**
 */
//bool ObjectType::isClass(void) const { return true; }

/**
 */
//const AbstractClass& ObjectType::asClass(void) const { return *this; }

/**
 */
//void *ObjectType::instantiate(void) const { return new Object(); }


/**
 * Type for top-level Object class.
 * @ingroup rtti
 */
//ObjectType& object_type = Single<ObjectType>::_;


/**
 * @class Class
 * @ref Type building for a class.
 * @param T		C++ type of the class this descriptor is used for.
 * @ingroup rtti
 */


/**
 * @class Enum
 * Type for an enumerated type. Recording an enumration in the RTTI system
 * requires several actions:
 * * defining an Enum descriptor,
 * * link the Enum descriptor with the type itself,
 * * providing support for other facilities (input/output, etc).
 *
 * To help providing this stuff, two macros are provided, @ref DECLARE_ENUM
 * is used in the header file and @ref DEFINE_ENUM in the source file.
 *
 * The code below represents an heade file where an enumerated type is
 * provided:
 * @code
 *	#include <elm/rtti.h>
 *
 *	typedef enum my_enum {
 *		A,
 *		B,
 *		C
 *	} my_enum;
 *	DECLARE_ENUM(my_enum);
 * @endcode
 *
 * The code below shows the source corresponding part:
 * @code
 *	elm::rtti::Enum my_enum_type(elm::rtti::make("my_enum")
 *		.value("A", A)
 *		.value("B", B)
 *		.value("C", C));
 *	DEFINE_ENUM(my_enum, my_enum_type);
 * @endcode
 *
 * @ingroup rtti
 */

/**
 * @class Enum::value
 * Abstract class representing a value of an enumerated type.
 * The corresponding concrete class, Enum, provides support
 * depending on the enumerated type itself.
 */

/**
 * @fn Enum::value::value(cstring name, int value);
 * Build a value.
 * @param name	Value name.
 * @param value	Value.
 */

/**
 * @fn cstring Enum::value::name(void) const;
 * Get the value name.
 * @return	Value name.
 */

/**
 * @fn int Enum::value::value(void) const;
 * Get the value.
 * @return	Value.
 */

/**
 * @class Enum::make
 * Maker to build an enumerated value @ref Enum.
 */

/**
 * @fn make& Enum::make::value(cstring name, int value);
 * Add a value.
 * @param name	Value name.
 * @param value	Value itself.
 */

/**
 * @fn make& Enum::make::alias(cstring name, int value);
 * @param name	Alias name.
 * @param value	Value itself.
 */

/**
 *
 */
Enum::Enum(const make& make): Type(make._name), _values(make._values) {
	_map.addAll(make._values);
	_map.addAll(make._aliases);
}

/**
 * Old-style enumerated type builder.
 * @param name		Full-qualified enumerated name.
 * @param values	Values of the enumerated type.
 */
Enum::Enum(cstring name, const Value values[]): Type(name) {
	for(int i = 0; values[i].name(); i++) {
		_values.add(values[i]);
		_map.add(values[i]);
	}
}

/**
 */
const Type& Enum::type(void) const {
	return *this;
}

/**
 * Get the value for a text.
 */
int Enum::valueFor(cstring text) const {
	for(Vector<Value>::Iter i = _map; i; i++)
		if((*i).name() == text)
			return (*i).value();
	return -1;
}

/**
 */
cstring Enum::nameFor(int value) const {
	for(Iter i = values(); i; i++)
		if((*i).value() == value)
			return (*i).name();
	return "";
}


/**
 * @fn Enum::Iter AbstractEnum::values(void) const;
 * Get the list of values.
 * @return	List of values.
 */

/**
 */
bool Enum::canCast(const Type *t) const {
	return t == this || t->isInt();
}

/**
 */
bool Enum::isEnum(void) const {
	return true;
}

/**
 */
const Enumerable& Enum::asEnum(void) const {
	return *this;
}

/**
 * @class Enum
 * Concrete representation of an enumerated type.
 * @ingroup rtti
 */

/**
 * @fn Enum::Enum(const make& make);
 * Constructor using a make object.
 * @param make	Object providing enumeration description.
 */

/**
 * @fn Enum::Enum(cstring name, const Value values[]);
 * Old-style constructor only provided for backward compatibility.
 */

/**
 * @def IS_CLASS_EXTEND(name, base)
 * Generate the content of a class to record it in the RTTI database.
 * @param name	Name of the class.
 * @param base	Base of the class.
 * @ingroup rtti
 */

/**
 * @def IS_CLASS(name, base)
 * Generate the content of a class to record it in the RTTI database.
 * @param name	Name of the class.
 * @ingroup rtti
 */

/**
 * @def CLASS_EXTEND(name, base)
 * All-in-one declaration macro for a class recorded in RTTI.
 * It substitutes to the `class` keyword and the class definition
 * must be ended by @ref END_CLASS.
 * @param name	Name of the class.
 * @param base	Base class.
 * @ingroup rtti
 */

/**
 * @def CLASS(name, base)
 * All-in-one declaration macro for a class recorded in RTTI.
 * It substitutes to the `class` keyword and the class definition
 * must be ended by @ref END_CLASS.
 * @param name	Name of the class.
 * @ingroup rtti
 */

/**
 * @def END_CLASS
 * Ends the declaration of a RTTI class declared by
 * @ref CLASS or @ref CLASS_EXTEND.
 * @ingroup rtti
 */

/**
 * @def IMPLEMENT(name)
 * Provide the implementation of a RTTI class. Must be put in the source
 * file of the class.
 * @param name	Name of the class.
 * @ingroup rtti
 */

/**
 * @def DECLARE_ENUM(name)
 * Declare an enumeration as supported by RTTI. Must be put just
 * after the enumeration declaration in the header file.
 * @param name	Enumeration type name.
 * @ingroup rtti
 */

/**
 * @def DEFINE_ENUM(type, desc)
 * Define the instance of type for an enumeration type.
 * Must be put in a source file.
 * @param type	The enumeration type.
 * @param desc	The instance of type describing the enumeration type.
 * @ingroup rtti.
 */

/**
 * @def BEGIN_ENUM(type)
 * Build the type instance representing an enumeration type.
 * Must be followed by the declaration of values of the type
 * and aliases as in @ref Enum::make and ended by @ref END_ENUM.
 * @param type	Enumeration type itself.
 * @ingroup rtti
 */

/**
 * @def END_ENUM
 * End the definition of the type instance of an enumeration type.
 * Must follow a @ref BEGIN_ENUM.
 * @ingroup rtti
 */

} }	// elm::rtti

package io.joynr.generator.cpp.communicationmodel
/*
 * !!!
 *
 * Copyright (C) 2011 - 2015 BMW Car IT GmbH
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import com.google.inject.Inject
import io.joynr.generator.cpp.util.JoynrCppGeneratorExtensions
import io.joynr.generator.cpp.util.TemplateBase
import org.franca.core.franca.FCompoundType
import io.joynr.generator.util.CompoundTypeTemplate
import io.joynr.generator.cpp.util.CppStdTypeUtil

class StdTypeCppTemplate implements CompoundTypeTemplate{

	@Inject
	private extension TemplateBase

	@Inject
	private extension JoynrCppGeneratorExtensions

	@Inject
	private extension CppStdTypeUtil

	override generate(FCompoundType type) '''
«val typeName = "Std" + type.joynrName»
«warning»

#include <sstream>
#include <string>
#include <typeinfo>

#include <QMetaEnum>

#include "«getIncludeOfStd(type)»"

#include "joynr/Reply.h"
#include "joynr/DeclareMetatypeUtil.h"
#include "joynr/Util.h"
#include "qjson/serializer.h"


«getNamespaceStarter(type)»



«IF !getMembersRecursive(type).empty»
«typeName»::«typeName»(
		«FOR member: getMembersRecursive(type) SEPARATOR ','»
			const «member.typeName» &«member.joynrName»
		«ENDFOR»
	):
		«IF hasExtendsDeclaration(type)»
			«val extendedType = getExtendedType(type)»
			Std«extendedType.joynrName»(
			«FOR member: getMembersRecursive(extendedType) SEPARATOR ','»
				«member.joynrName»
			«ENDFOR»
			),
		«ENDIF»
		«FOR member: getMembers(type) SEPARATOR ','»
			«member.joynrName»(«member.joynrName»)
		«ENDFOR»
{
}
«ENDIF»

«IF !getMembers(type).isEmpty»
// Copy Constructor
«typeName»::«typeName»(const «typeName»& other) :
		«IF hasExtendsDeclaration(type)»
			Std«getExtendedType(type).joynrName»(other),
		«ENDIF»
		«FOR member: getMembers(type) SEPARATOR ','»
			«member.joynrName»(other.«member.joynrName»)
		«ENDFOR»
{
}
«ENDIF»

bool «typeName»::operator!=(const «typeName»& other) const {
	return !(*this==other);
}

bool «typeName»::operator==(const «typeName»& other) const {
	if (typeid(*this) != typeid(other)) {
		return false;
	}

	return
		«FOR member: getMembers(type)»
			this->«member.joynrName» == other.«member.joynrName» &&
		«ENDFOR»
		«IF hasExtendsDeclaration(type)»
			«getExtendedType(type).typeName»::operator==(other);
		«ELSE»
			true;
		«ENDIF»
}

«FOR member: getMembers(type)»
	«val joynrName = member.joynrName»
	«IF isEnum(member.type) && ! isArray(member)»
		std::string «typeName»::get«joynrName.toFirstUpper»Internal() const {
			QMetaEnum metaEnum = «member.typeName.substring(0, member.typeName.length-6)»::staticMetaObject.enumerator(0);
			return metaEnum.valueToKey(this->«joynrName»);
		}

	«ENDIF»
«ENDFOR»

std::string «typeName»::toString() const {
	std::ostringstream typeAsString;
	typeAsString << "«typeName»{";
	«IF hasExtendsDeclaration(type)»
		typeAsString << «getExtendedType(type).typeName»::toString();
		«IF !getMembers(type).empty»
		typeAsString << ", ";
		«ENDIF»
	«ENDIF»
	«FOR member: getMembers(type) SEPARATOR "\ntypeAsString << \", \";"»
		«val memberName = member.joynrName»
		«IF isArray(member)»
			typeAsString << " unprinted List «memberName»  ";
		«ELSEIF isByteBuffer(member.type)»
			typeAsString << " unprinted ByteBuffer «memberName»  ";
		«ELSEIF isString(getPrimitive(member.type))»
			typeAsString << "«memberName»:" + get«memberName.toFirstUpper»();
		«ELSEIF isEnum(member.type)»
			typeAsString << "«memberName»:" + get«memberName.toFirstUpper»Internal();
		«ELSEIF isComplex(member.type)»
			typeAsString << "«memberName»:" + get«memberName.toFirstUpper»().toString();
		«ELSE»
			typeAsString << "«memberName»:" + std::to_string(get«memberName.toFirstUpper»());
		«ENDIF»
	«ENDFOR»
	typeAsString << "}";
	return typeAsString.str();
}

// printing «typeName» with google-test and google-mock
void PrintTo(const «typeName»& «typeName.toFirstLower», ::std::ostream* os) {
	*os << "«typeName»::" << «typeName.toFirstLower».toString();
}
«getNamespaceEnder(type)»
'''
}
package io.joynr.generator.js.communicationmodel

/*
 * !!!
 *
 * Copyright (C) 2011 - 2016 BMW Car IT GmbH
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
import com.google.inject.assistedinject.Assisted
import io.joynr.generator.js.util.GeneratorParameter
import io.joynr.generator.js.util.JSTypeUtil
import io.joynr.generator.js.util.JoynrJSGeneratorExtensions
import io.joynr.generator.templates.CompoundTypeTemplate
import io.joynr.generator.templates.util.NamingUtil
import java.util.Date
import org.franca.core.franca.FCompoundType
import org.franca.core.franca.FStructType
import org.franca.core.franca.FUnionType

class CompoundTypeGenerator extends CompoundTypeTemplate {

	@Inject extension JSTypeUtil
	@Inject extension GeneratorParameter
	@Inject private extension NamingUtil
	@Inject private extension JoynrJSGeneratorExtensions

	@Inject
	new(@Assisted FCompoundType type) {
		super(type)
	}

	override generate() '''
		«IF type instanceof FStructType»
			«generateStructType(type)»
		«ELSEIF type instanceof FUnionType»
			«generateUnionType(type as FUnionType)»
		«ENDIF»
	'''

	def generateUnionType(FUnionType type) '''
		//TODO generate union type «type.joynrName»
	'''

	def generateStructType(FStructType type) '''
		«val generationDate = (new Date()).toString»
		/**
		 * This is the generated struct type «type.joynrName»: DOCS GENERATED FROM INTERFACE DESCRIPTION
		 * Generation date: «generationDate»
		 */
		(function(undefined) {
			/**
			 * @name «type.joynrName»
			 * @constructor
			 *
			 * @classdesc
			 * This is the generated struct type «type.joynrName»: DOCS GENERATED FROM INTERFACE DESCRIPTION
			 * <br/>Generation date: «generationDate»
			 «appendJSDocSummaryAndWriteSeeAndDescription(type, "* ")»
			 *
			 * @param {Object} members - an object containing the individual member elements
			 «val members = getMembersRecursive(type)»
			 «FOR member : members»
			 * @param {«member.jsdocTypeName»} members.«member.joynrName» - «IF member.comment != null»«FOR comment : member.comment.elements»«comment.
				comment.replaceAll("\n", "\n" + "* ")»«ENDFOR»«ENDIF»
			 «ENDFOR»
			 * @returns {«type.joynrName»} a new instance of a «type.joynrName»
			 */
			var «type.joynrName» = function «type.joynrName»(members) {
				if (!(this instanceof «type.joynrName»)) {
					// in case someone calls constructor without new keyword (e.g. var c = Constructor({..}))
					return new «type.joynrName»(members);
				}

				/**
				 * Used for serialization.
				 * @name «type.joynrName»#_typeName
				 * @type String
				 * @readonly
				 */
				Object.defineProperty(this, "_typeName", {
					configurable : false,
					writable : false,
					enumerable : true,
					value : "«type.joynrTypeName»"
				});
				«IF type.base != null»

				/**
				 * Parent class.
				 * @name «type.joynrName»#_extends
				 * @type String
				 * @readonly
				 */
				Object.defineProperty(this, "_extends", {
					configurable : false,
					writable : false,
					enumerable : false,
					value : "«type.base.joynrTypeName»"
				});
				«ENDIF»

				«FOR member : members»
					/**
					 * «IF member.comment != null»«FOR comment : member.comment.elements»«comment.
						comment.replaceAll("\n", "\n" + "* ")»«ENDFOR»«ENDIF»
					 * @name «type.joynrName»#«member.joynrName»
					 * @type «member.jsdocTypeName»
					 */
				«ENDFOR»
				Object.defineProperty(this, 'checkMembers', {
					enumerable: false,
					value: function checkMembers(check) {
						«FOR member : members»
						check(this.«member.joynrName», «member.checkPropertyTypeName», "members.«member.joynrName»");
						«ENDFOR»
					}
				});

				if (members !== undefined) {
					«FOR member : members»
					this.«member.joynrName» = members.«member.joynrName»;
					«ENDFOR»
				}

			};

			/**
			 * @name «type.joynrName»#MAJOR_VERSION
			 * @constant {Number}
			 * @default «majorVersion»
			 * @summary The MAJOR_VERSION of the struct type «type.joynrName» is GENERATED FROM THE INTERFACE DESCRIPTION
			 */
			Object.defineProperty(«type.joynrName», 'MAJOR_VERSION', {
				enumerable: false,
				configurable: false,
				writable: false,
				readable: true,
				value: «majorVersion»
			});
			/**
			 * @name «type.joynrName»#MINOR_VERSION
			 * @constant {Number}
			 * @default «minorVersion»
			 * @summary The MINOR_VERSION of the struct type «type.joynrName» is GENERATED FROM THE INTERFACE DESCRIPTION
			 */
			Object.defineProperty(«type.joynrName», 'MINOR_VERSION', {
				enumerable: false,
				configurable: false,
				writable: false,
				readable: true,
				value: «minorVersion»
			});

			var memberTypes = {
				«FOR member : members SEPARATOR ","»
				«member.joynrName»: function() { return "«member.joynrTypeName»"; }
				«ENDFOR»
			};
			Object.defineProperty(«type.joynrName», 'getMemberType', {
				enumerable: false,
				value: function getMemberType(memberName) {
					if (memberTypes[memberName] !== undefined) {
						return memberTypes[memberName]();
					}
					return undefined;
				}
			});

			«IF requireJSSupport»
			// AMD support
			if (typeof define === 'function' && define.amd) {
				define(«type.defineName»["joynr"], function (joynr) {
					«type.joynrName».prototype = new joynr.JoynrObject();
					«type.joynrName».prototype.constructor = «type.joynrName»;
					joynr.addType("«type.joynrTypeName»", «type.joynrName»);
					return «type.joynrName»;
				});
			} else if (typeof exports !== 'undefined' ) {
				if ((module !== undefined) && module.exports) {
					exports = module.exports = «type.joynrName»;
				} else {
					// support CommonJS module 1.1.1 spec (`exports` cannot be a function)
					exports.«type.joynrName» = «type.joynrName»;
				}
				var joynr = requirejs("joynr");
				«type.joynrName».prototype = new joynr.JoynrObject();
				«type.joynrName».prototype.constructor = «type.joynrName»;

				joynr.addType("«type.joynrTypeName»", «type.joynrName»);
			} else {
				«type.joynrName».prototype = new window.joynr.JoynrObject();
				«type.joynrName».prototype.constructor = «type.joynrName»;
				window.joynr.addType("«type.joynrTypeName»", «type.joynrName»);
				window.«type.joynrName» = «type.joynrName»;
			}
			«ELSE»
				//we assume a correct order of script loading
			«type.joynrName».prototype = new window.joynr.JoynrObject();
			«type.joynrName».prototype.constructor = «type.joynrName»;
			window.joynr.addType("«type.joynrTypeName»", «type.joynrName»);
			window.«type.joynrName» = «type.joynrName»;
			«ENDIF»
		})();
	'''
}

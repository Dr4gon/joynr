package io.joynr.generator.cpp.proxy
/*
 * !!!
 *
 * Copyright (C) 2011 - 2016 BMW Car IT GmbH
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 *
 *  You may obtain a copy of the License at
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
import io.joynr.generator.cpp.util.CppInterfaceUtil
import io.joynr.generator.cpp.util.CppStdTypeUtil
import io.joynr.generator.cpp.util.JoynrCppGeneratorExtensions
import io.joynr.generator.cpp.util.TemplateBase
import io.joynr.generator.templates.InterfaceTemplate
import io.joynr.generator.templates.util.MethodUtil
import io.joynr.generator.templates.util.NamingUtil

class InterfaceFireAndForgetProxyCppTemplate extends InterfaceTemplate {
	@Inject	extension JoynrCppGeneratorExtensions
	@Inject extension TemplateBase
	@Inject extension CppStdTypeUtil
	@Inject private extension NamingUtil
	@Inject private extension MethodUtil
	@Inject private extension CppInterfaceUtil

	override generate()
'''
«val interfaceName =  francaIntf.joynrName»
«val className = interfaceName + "Proxy"»
«val fireAndForgetClassName = interfaceName + "FireAndForgetProxy"»
«warning()»

#include "«getPackagePathWithJoynrPrefix(francaIntf, "/")»/«fireAndForgetClassName».h"

«FOR datatype: getDataTypeIncludesFor(francaIntf)»
	#include «datatype»
«ENDFOR»


«getNamespaceStarter(francaIntf)»
«fireAndForgetClassName»::«fireAndForgetClassName»(
		std::shared_ptr<const joynr::system::RoutingTypes::Address> messagingAddress,
		joynr::ConnectorFactory* connectorFactory,
		joynr::IClientCache *cache,
		const std::string &domain,
		const joynr::MessagingQos &qosSettings,
		bool cached
) :
		joynr::ProxyBase(connectorFactory, cache, domain, qosSettings, cached),
		«className»Base(messagingAddress, connectorFactory, cache, domain, qosSettings, cached)
{
}

«FOR method: getMethods(francaIntf).filter[fireAndForget]»
	«var methodName = method.name»
	«val outputUntypedParamList = getCommaSeperatedUntypedOutputParameterList(method)»
	«var params = getCommaSeperatedUntypedInputParameterList(method)»
	/*
	 * «methodName»
	 */
	«produceFireAndForgetMethodSignature(method, fireAndForgetClassName)»
	{
		if (connector==nullptr){
			«val errorMsg = "proxy cannot invoke " + methodName + " because the communication end partner is not (yet) known"»
			JOYNR_LOG_WARN(logger, "«errorMsg»");
				exceptions::JoynrRuntimeException error("«errorMsg»");
				throw error;
		}
		else{
			return connector->«methodName»(«outputUntypedParamList»«IF method.outputParameters.size > 0 && method.inputParameters.size > 0», «ENDIF»«params»);
		}
	}
«ENDFOR»
«getNamespaceEnder(francaIntf)»
'''
}

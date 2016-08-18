# KAI Config

Configuration of the system. Defaults are sane, but can overridden:

* KAI_USE_NAMESPACES
Turn this off and KAI tries to make thing work without
using namespaces.

Hey, it could still work. But in general this is assumed to be true.

* KAI_NAMESPACE_NAME
The name that KAI lives in. Defaults to **kai**. 
Yes, you really change the namespace of the entire library
by changing this.

* KAI_DEBUG_TRACE
Set trace level. Defaults to 0. 9 is very noisy.

* KAI_ENABLE_EXCEPTIONS
It defauts to ON, so if you do nothing you're gold.
__HOWEVER__ If you really don't want exceptions, turn this off. I haven't
tried a build with this unset for 6 years, but you will be able to
get a build that doesn't use exceptions pretty easily if you turn this
off.





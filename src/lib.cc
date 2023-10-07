#include <node_api.h>

#include "lib.h"

static napi_value crash(napi_env env, napi_callback_info info) {
    napi_throw_error(env, NULL, "Throwing an error from C++, hooray!!");
    return nullptr;
}

napi_value create_addon(napi_env env) {
  napi_value result;
  NAPI_CALL(env, napi_create_object(env, &result));

  napi_value exported_function;
  NAPI_CALL(env, napi_create_function(env,
                                      "crash",
                                      NAPI_AUTO_LENGTH,
                                      crash,
                                      nullptr,
                                      &exported_function));

  NAPI_CALL(env, napi_set_named_property(env,
                                         result,
                                         "crash",
                                         exported_function));

  return result;
} 


NAPI_MODULE_INIT() {
    return create_addon(env);
}
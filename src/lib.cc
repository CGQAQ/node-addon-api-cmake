#include <iostream>

#include "lib.hpp"

/**
 * \remarks
 * Throws an exception from C++.
*/
static napi_value crash(napi_env env, napi_callback_info info)
{
	napi_throw_error(env, NULL, "Throwing an error from C++, hooray!!");
	return nullptr;
}

/**
 * \remarks
 * Adds all the arguments passed to the function. up to 10 arguments.
*/
static napi_value add(napi_env env, napi_callback_info info)
{
	static constexpr size_t kArgCount{10};
	size_t argc{kArgCount};
	napi_value argv[kArgCount];
	napi_value this_arg{nullptr};
	void *data{nullptr};
	NAPI_CALL(env, napi_get_cb_info(env, info, &argc, argv, &this_arg, &data));

	if (argc < 2)
	{
		napi_throw_error(env, NULL, "Wrong number of arguments, expected >= 2");
		return nullptr;
	}

	int32_t acc{0};
	for (size_t i{0}; i < kArgCount; ++i)
	{
		napi_handle_scope scope{};
		napi_open_handle_scope(env, &scope);

		napi_valuetype valuetype;
		NAPI_CALL(env, napi_typeof(env, argv[i], &valuetype));
		if (valuetype == napi_number)
		{
			int32_t value{0};
			NAPI_CALL(env, napi_get_value_int32(env, argv[i], &value));
			acc += value;
		}
		else if (valuetype == napi_undefined)
		{
			// do nothing
		}
		else if (valuetype != napi_number)
		{
			napi_throw_error(env, NULL, "Wrong argument type, expected number");
			return nullptr;
		}

		napi_close_handle_scope(env, scope);
	}

	napi_value result{};
	NAPI_CALL(env, napi_create_int32(env, acc, &result));

	return result;
}

/**
 * \remarks
 * Wraps the first argument in a promise and returns it.
*/
static napi_value wrap_arg_in_promise(napi_env env, napi_callback_info info)
{
	size_t argc{1};
	napi_value argv[1]{nullptr};
	napi_value this_arg{nullptr};
	void *data{nullptr};
	NAPI_CALL(env, napi_get_cb_info(env, info, &argc, argv, &this_arg, &data));

	
	napi_value promise{};
	napi_deferred deferred{};
	NAPI_CALL(env, napi_create_promise(env, &deferred, &promise));
	NAPI_CALL(env, napi_resolve_deferred(env, deferred, argv[0]));

	return promise;
}

napi_value create_addon(napi_env env)
{
	napi_value result;
	NAPI_CALL(env, napi_create_object(env, &result));

	napi_value fn_crash;
	NAPI_CALL(env, napi_create_function(env,
										"crash",
										NAPI_AUTO_LENGTH,
										crash,
										nullptr,
										&fn_crash));
	NAPI_CALL(env, napi_set_named_property(env,
										   result,
										   "crash",
										   fn_crash));

	napi_value fn_add;
	NAPI_CALL(env, napi_create_function(env,
										"add",
										NAPI_AUTO_LENGTH,
										add,
										nullptr,
										&fn_add));
	NAPI_CALL(env, napi_set_named_property(env,
										   result,
										   "add",
										   fn_add));

	napi_value fn_wrap_in_promise;
	NAPI_CALL(env, napi_create_function(
					   env,
					   "wrapInPromise",
					   NAPI_AUTO_LENGTH,
					   wrap_arg_in_promise,
					   nullptr,
					   &fn_wrap_in_promise));

	NAPI_CALL(env, napi_set_named_property(
					   env,
					   result,
					   "wrapInPromise",
					   fn_wrap_in_promise));
	return result;
}

NAPI_MODULE_INIT()
{
	return create_addon(env);
}

<?php
/**
* Reads wp-config.php to get Mangopay parms configuration. Returns MangoPay\MangoPayApi() or null.
*
*	Expected section in configuration file.
*	// Mangopay info
*		define('MWP_client_id', 'ciccoopfunding');
*		define('MWP_password', 'Rn6qS28KvoxxDRkyEmFrAK6vWQnwf6ik9weM6QtAdVxtcag1PT');
*		define('MWP_temp_path', '/tmp/');
*		define('MWP_base_path', 'https://api.sandbox.mangopay.com');
**/



function mwp_get_api () {
	$ABSPATH = dirname(dirname(dirname(dirname(dirname(__FILE__)))));
	require_once $ABSPATH . "/wp-load.php";
	
	require_once __DIR__ . "/MangoPaySDK/mangoPayApi.inc";
	$api = new MangoPay\MangoPayApi();

	$api->Config->ClientId = MWP_client_id;
	$api->Config->ClientPassword = MWP_password;
	$api->Config->TemporaryFolder = MWP_temp_path;
	$api->Config->BaseUrl = MWP_base_path;

	return $api;

}

?>



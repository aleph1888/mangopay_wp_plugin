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

namespace mwp;
	include 'MangoPaySDK/mangoPayApi.inc';
	$ABSPATH = dirname(dirname(dirname(dirname(dirname(__FILE__)))));
	require_once $ABSPATH . "/wp-load.php";
	require_once __DIR__ . "/mwp_errors.inc";

use MangoPay as m;

class mwp_api extends m\MangoPayApi {

	static private $instance;

	public static function get_instance( $s = null) {

		$_SESSION["MWP_API_ERROR"] = null;

		if ( self::$instance == null ) {

			self::$instance = new mwp_api;

			self::$instance->Config->ClientId = MWP_client_id;
			self::$instance->Config->ClientPassword = MWP_password;
			self::$instance->Config->TemporaryFolder = MWP_temp_path;
			self::$instance->Config->BaseUrl = MWP_base_path;

		}

		return self::$instance;

	}

}

?>



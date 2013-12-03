<?php

require_once(dirname(__FILE__) . "/config.inc");
require_once(dirname(__FILE__) . "/lib/common.inc");

// POST request to create a user	
function generateNewUser()
{
	$body = json_encode(array("FirstName" => "John", "LastName" => "Doe", "Email" => "john.doe@unknow.com", "IP" => "127.0.0.1", "CanRegisterMeanOfPayment" => "true"));
	$user = request("users", "POST", $body);
	
	print_r  ("\n" . "DataCreation : New USER Create ".$user -> ID);
	
	return $user;
}


function generateNewWallet($user)
{
	$body = json_encode(array("Owners" => array($user -> ID)));
	$wallet = request("wallets", "POST", $body);
	
	print_r  ("\n" . "DataCreation : New WALLET Create ".$user -> ID);
	
	return $wallet;
}

<?php

/**
* Sends a contribution to Mangopay
*
**/

require_once (dirname(__FILE__) . "/lib/common.inc");

// Retrieve parameters
$parameters = array(
		"UserID", "WalletID", "ReturnURL", "Amount", "ReturnURL", "Tag", "ClientFeeAmount", "TemplateURL", 
		"RegisterMeanOfPayment", "PaymentCardID", "Culture", "PaymentMethodType", "Type");

$array = array();
for ($i = 0; $i < count($parameters) ; $i++) {
	if(isset($_REQUEST[$parameters[$i]])){
		$array[$parameters[$i]] = $_REQUEST[$parameters[$i]];
	}
}

// Convert format
$body = json_encode($array);

$contribution = request("contributions", "POST", $body);

/*
* Redirect to url of payment
*/
if ($contribution != null) {
	print "<a href=" . $contribution -> PaymentURL . ">Aller au paiement</a>";
	wp_redirect( $contribution -> PaymentURL );
} else {
	print "contribution is not create";
}


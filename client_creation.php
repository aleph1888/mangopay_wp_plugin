
<?php
require_once (dirname(__FILE__) . '/plugin/mangopay/MangoPaySDK/mangoPayApi.inc');
$api = new MangoPay\MangoPayApi();

// Change this fields to set you own client.
$client = $api->Clients->Create(
    '<ClientID>', 
    '<Name>', 
    '<Mail>'
);

// you receive your password here, note it down and keep in secret
print($client->Passphrase);

?>

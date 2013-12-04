
<?php
require_once (dirname(__FILE__) . '/plugin/mangopay/MangoPaySDK/mangoPayApi.inc');
$api = new MangoPay\MangoPayApi();

$client = $api->Clients->Create(
    'ciccoopfunding', 
    'coopfunding.net', 
    'aleph@riseup.net'
);

// you receive your password here, note it down and keep in secret
print($client->Passphrase);

?>

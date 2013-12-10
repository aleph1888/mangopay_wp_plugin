<?php
/**
* Mangopay paymentDirect process
*	$_POST {user_id, Amount, wallet_id, cardNumber, cardDate, cardCvx}
**/

session_start();

$ABSPATH = dirname(dirname(dirname(dirname(__FILE__))));
require_once $ABSPATH . "/wp-load.php";

//Error display function
function mwp_error_redirect ( $message ) {
	return _e( $message, 'mangopay_wp_plugin') . " " . $_SESSION["MWP_API_ERROR"];

}

//Main process function
function mwp_process_contribution () {

	require_once ( __DIR__ . "/includes/mwp_pay.inc");
	$po = new mwp_pay;

	//Verify amount
	if ( ! $_POST['mwp_Amount'] > 0  )
		return mwp_error_redirect ( 'mangopay_bad_amount' );

	//USER
	if ( ! $po -> mwp_new_user () )
		return mwp_error_redirect ( 'mangopay_bad_user' );

	//CARD
	$data = $po -> mwp_preregister_card ();
	if ( !$data ) 
		return mwp_error_redirect ( "mango_pay_bad_card");

	//Return url
	foreach($data as $key=>$val)  
		$post_str .= $key.'='.urlencode($val).'&'; 
	$post_str = substr($post_str, 0, -1); 

	//Send request
	$ch = curl_init();
	curl_setopt($ch, CURLOPT_URL, $po->card_registration->CardRegistrationURL);

	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
	curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);
	curl_setopt($ch, CURLOPT_POST, true);
	curl_setopt($ch, CURLOPT_POSTFIELDS, $post_str ); 
	$output = curl_exec($ch);
	$info = curl_getinfo($ch);
	curl_close($ch);

	if ( ! $po -> mwp_validate_card ( $output ) )
		return mwp_error_redirect ( "mango_pay_bad_card" );

	$po -> mwp_wallet_for_post ( $_POST['mwp_post_id' ] );

	return $po -> mwp_payIn ( $_POST['mwp_Amount'], 0 );

}

//Show data
	_wp_admin_bar_init();
	get_header();
?>
	<div style="margin: 0 auto; width:900px; height:500px">
		<p><?php echo mwp_process_contribution(); ?></p>
		<a href="<?php echo get_permalink ( $_POST['mwp_post_id' ] ) ?>"><?php _e( 'back', 'mangopay_wp_plugin'); ?></a>
	</div>
<?php
get_sidebar();
get_footer(); 
?>



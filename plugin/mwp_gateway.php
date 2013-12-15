<?php
/**
* Mangopay paymentDirect process
*	$_REQUEST {user_id, Amount, wallet_id, cardNumber, cardDate, cardCvx}
**/

session_start();

$ABSPATH = dirname(dirname(dirname(dirname(__FILE__))));
require_once $ABSPATH . "/wp-load.php";

//Error display function
function mwp_error_redirect ( $message ) {
	return _e( $message, 'mangopay_wp_plugin') . " <br>\n " . $_SESSION["MWP_API_ERROR"];

}

//Main process function
function mwp_process_contribution () {

	//instantiate object
	require_once ( __DIR__ . "/includes/mwp_pay.inc");
	$autosave = true;
	$po = new mwp\mwp_pay ( wp_get_current_user (), $autosave ) ;

	//Verify user
	if ( ! $po -> user -> mangopay_id )
		return mwp_error_redirect ( 'mangopay_id_missing' );

	//Verify amount
	if ( ! $_REQUEST['mwp_Amount'] > 0 )
		return mwp_error_redirect ( 'missing_amount' );

	//CARD
	$needs_to_register_card = ! $po -> user -> card_id;

	if ( $needs_to_register_card ) {
		$data = $po -> mwp_preregister_card ();

		if ( ! $data ) 
			return mwp_error_redirect ( "mango_pay_bad_card.preregister");

		$output = mwp_send_to_token_server ( $data, $po->card_registration->CardRegistrationURL );

		if ( ! $po -> mwp_validate_card ( $output ) )
			return mwp_error_redirect ( "mango_pay_bad_card.validate: " . $output );
	}

	$po -> mwp_wallet_for_post ( $_REQUEST['mwp_post_id' ] );

	return $po -> mwp_payIn ( $_REQUEST['mwp_Amount'], 0 );

}

//Show data
_wp_admin_bar_init();
get_header();

//Url to return if errors
$url_params = "mwp_Amount={$_REQUEST['mwp_Amount']}&mwp_post_id={$_REQUEST['mwp_post_id']}";
$back_to_url = plugin_dir_url(__FILE__) . "mwp_payform.php?{$url_params}";
$post_title = get_the_title( $_REQUEST['mwp_post_id'] );
?>
	<div style="margin: 0 auto; width:900px; height:500px">
		<p><?php echo mwp_process_contribution(); ?></p>
		<a href="<?php echo $back_to_url ?>"><?php _e( 'back', 'mangopay_wp_plugin'); ?></a><br>
		<a href="<?php echo get_permalink ( $_REQUEST['mwp_post_id'] ) ?>"><?php echo $post_title ?></a>
	</div>
<?php
get_sidebar();
get_footer(); 

function mwp_send_to_token_server ( $data, $url ) {

	//Url encode
	foreach($data as $key=>$val)  
		$post_str .= $key.'='.urlencode($val).'&'; 
	$post_str = substr($post_str, 0, -1); 

	//Send request
	$ch = curl_init();
	curl_setopt($ch, CURLOPT_URL, $url);

	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
	curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);
	curl_setopt($ch, CURLOPT_POST, true);
	curl_setopt($ch, CURLOPT_POSTFIELDS, $post_str ); 
	$output = curl_exec($ch);
	$info = curl_getinfo($ch);
	curl_close($ch);

	return $output;

}

?>

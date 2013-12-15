<?php

function mwp_get_fields ( $type ) {
	switch ( $type ) {
		case "Natural":
			$yFields =  array ( "Email", "FirstName", "LastName", "Birthday", "Nationality", "CountryOfResidence" );
			break;
		case "Legal":
			$yFields = array ("Email", "Name", 
				"LegalPersonType", "LegalRepresentativeFirstName", "LegalRepresentativeLastName", 
				"LegalRepresentativeBirthday", "LegalRepresentativeNationality", 
				"LegalRepresentativeCountryOfResidence");
			break;
		case "register":
			$yFields = array ("cardNumber", "cardDate", "cardCvx");
			break;
		case "pay":
			$yFields = array ("Alias", "Post");
			break;
		case "bank":
			$yFields = array ("OwnerName", "OwnerAddress", "IBAN", "BIC" );
	}
	return $yFields;
}

//Return true or false wether any of the specified fields has change between post and object
function mwp_has_changed_fields ( $yFields, $user ) {
	foreach ( $yFields as $field ) {
		$has_changed = ( $_POST["mwp_{$field}"] != $user -> $field );
		if ( $has_changed )
			return true;
	}
	return false;
}


?>

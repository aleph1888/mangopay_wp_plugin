<?php
	function mwp_get_fields ( $type ) {
		switch ( $type ) {
			case "natural":
				$yFields =  array ( "Email", "FirstName", "LastName", "Birthday", "Nationality", "CountryOfResidence" );
				break;
			case "legal":
				$yFields = array ("Email", "Name", 
					"LegalPersonType", "LegalRepresentativeFirstName", "LegalRepresentativeLastName", 
					"LegalRepresentativeBirthday", "LegalRepresentativeNationality", 
					"LegalRepresentativeCountryOfResidence");
				break;
			case "register":
				$yFields = array ("cardNumber", "cardDate", "cardCvx");
				break;
			case "pay":
				$yFields = array ("Alias", "Amount", "Post");
				break;
		}
		return $yFields;
	}
?>

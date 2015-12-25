function getQueryParam(variable, defaultValue) {
	// Find all URL parameters
	var query = location.search.substring(1);
	var vars = query.split('&');
	for (var i = 0; i < vars.length; i++) {
		var pair = vars[i].split('=');
		// If the query variable parameter is found, decode it to use and return it for use
		if (pair[0] === variable) {
			return decodeURIComponent(pair[1]);
		}
	}
	return defaultValue || false;
}

function submit() {
	var cals = [];
	for(var id in calendarCheckboxes) {
		if(document.getElementById(id).checked) {
			cals.push(document.getElementById(id).value);
		}
	}
	
	var settings = {
		"calendar": cals.join(),
		"color-hour": document.getElementById("in-hour").value,
		"color-minute": document.getElementById("in-min").value,
		"color-appointment": document.getElementById("in-appt").value,
		"color-circle": document.getElementById("in-circle").value,
		"color-background": document.getElementById("in-back").value,
		"color-date": document.getElementById("in-date").value,
		"oauth2-refresh-token": refresh_token,
	}
	
	// Set the return URL depending on the runtime environment
	var return_to = getQueryParam('return_to', 'pebblejs://close#');
	document.location = decodeURIComponent(return_to) + 
		encodeURIComponent(JSON.stringify(settings));
}

function currentValues() {
	var config = getQueryParam('current', false);
	console.log("setting current values " + config);
	if (config == false) {
		return;
	}
	config = JSON.parse(config);
	document.getElementById("in-cal").value = config["calendar"].replace("0x","#");
	document.getElementById("in-hour").value = config["color-hour"].replace("0x","#");
	document.getElementById("in-min").value = config["color-minute"].replace("0x","#");
	document.getElementById("in-appt").value = config["color-appointment"].replace("0x","#");
	document.getElementById("in-circle").value = config["color-circle"].replace("0x","#");
	document.getElementById("in-back").value = config["color-background"].replace("0x","#");
	document.getElementById("in-date").value = config["color-date"].replace("0x","#");
	have_refresh_token = config["oauth2-have-token"];
}

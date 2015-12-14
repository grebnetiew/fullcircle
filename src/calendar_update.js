/*
  This app is run on the phone to periodically update the internal calendar,
  and push the results to the watch.
*/

function zeros() {
  var a = new Array(40);
  for(var i = 0; i != 40; ++i) {
    a[i] = 0;
  }
  return a;
}

var appointments = zeros();

Pebble.addEventListener('ready', function(){
  setInterval(calendarUpdate, 900000);
  calendarUpdate();
});

function retrieveAppointments() {
  var apiClientID = '1337';
  var apiClientSecret = 'LOL';
  var apiKey = 'AIzaSyDaa5bCvVpTkWOwpdMu_3mwDMF4x92f240';
  var calendar = "e.r.a.weitenberg@gmail.com";//localStorage.getItem(0);
  var timeMin = new Date();
  var timeMax = new Date(timeMin);
  timeMax.setHours(timeMax.getHours() + 12);
  var query = {
    "timeMin": timeMin.toISOString(),
    "timeMax": timeMax.toISOString(),
    "items": [
      { "id": calendar },
    ]
  };
  
  //console.log("Asking for " + JSON.stringify(query));
  
  var req = new XMLHttpRequest();
  req.open('POST', "https://www.googleapis.com/calendar/v3/freeBusy?key=" + apiKey, false);
  req.setRequestHeader('Content-Type', 'application/json');
  req.send(JSON.stringify(query));
  
  var response = JSON.parse(req.responseText);
  
  //console.log("Answer was " + req.responseText);
  
  if (Object.keys(response)[0] == "error" || !response.calendars[calendar]) {
    // An error occurred, or the calendar is simply empty.
    console.log("Something might be wrong. The error was " + JSON.stringify(response.error.errors[0]));
    return zeros();
  }
  if (response.calendars[calendar].errors) {
    console.log("Errors from Google Calendar: " + 
                JSON.stringify(response.calendars[calendar].errors));
    return zeros();
  }
  var freebusy = response.calendars[calendar].busy;
  if (!freebusy) {
    console.log("There was no free-busy information.");
    return zeros();
  }
  
  var newAppointments = zeros();
  for(var i = 0; i != Math.min(10, freebusy.length); ++i) {
    newAppointments[4*i + 0] = (new Date(freebusy[i].start)).getHours();
    newAppointments[4*i + 1] = (new Date(freebusy[i].start)).getMinutes();
    newAppointments[4*i + 2] = (new Date(freebusy[i].end)).getHours();
    newAppointments[4*i + 3] = (new Date(freebusy[i].end)).getMinutes();
  }
  
  return newAppointments;
}

function sendAppointments(appointments) { 
  Pebble.sendAppMessage( { '1': appointments },
    function(e) {
      console.log('Successfully delivered message ' + JSON.stringify(appointments) + ' with transactionId=' + e.data.transactionId);
    },
    function(e) {
      console.log('Unable to deliver message with transactionId=' + e.data.transactionId  + ' Error is: ' + e.error.message);
    }
  );
}

function calendarUpdate() {
  var newAppointments = retrieveAppointments();
  if (JSON.stringify(newAppointments) != JSON.stringify(appointments)) {
    appointments = newAppointments;
    sendAppointments(appointments);
  }
}

function hexToInt(hex) {
  return parseInt(hex.replace("#", "0x")) || 0;
}

// Show config page when needed
Pebble.addEventListener('showConfiguration', function(e) {
  Pebble.openURL('http://saffier.no-ip.org/fullcircle/config.html');
});

// Send the config after it's been updated
Pebble.addEventListener('webviewclosed', function(e) {
  // Decode and parse config data as JSON
  var config_data = JSON.parse(decodeURIComponent(e.response));
  console.log('Config window returned: ', JSON.stringify(config_data));

  localStorage.setItem(0, config_data.calendar);
  // Prepare AppMessage payload
  var dict = {
    "11": hexToInt(config_data["color-hour"]),
    "12": hexToInt(config_data["color-minute"]),
    "13": hexToInt(config_data["color-appointment"]),
    "14": hexToInt(config_data["color-circle"]),
    "15": hexToInt(config_data["color-background"]),
  };

  // Send settings to Pebble watchapp
  Pebble.sendAppMessage(dict, function(){
    console.log('Sent config data to Pebble');  
  }, function() {
    console.log('Failed to send config data!');
  });
  calendarUpdate();
});
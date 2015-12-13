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

Pebble.addEventListener('ready', function(e) {
  console.log('JavaScript app ready and running!');
  appointments = retrieveAppointments();
  sendAppointments();
  setTimeout(calendarUpdate, 900000);
});

function retrieveAppointments() {
  var apiClientID = '1337';
  var apiClientSecret = 'LOL';
  var apiKey = 'AIzaSyDaa5bCvVpTkWOwpdMu_3mwDMF4x92f240';
  var calendar = "eweitenberg@gmail.com";
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
  
  if (Object.keys(response)[0] == "error") {
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
}

function sendAppointments() { 
  Pebble.sendAppMessage( { '1': appointments },
    function(e) {
      console.log('Successfully delivered message ' + appointments.toString() + ' with transactionId=' + e.data.transactionId);
    },
    function(e) {
      console.log('Unable to deliver message with transactionId=' + e.data.transactionId  + ' Error is: ' + e.error.message);
    }
  );
}

function calendarUpdate() {
  var newAppointments = retrieveAppointments();
  if (newAppointments.toString() != appointments.toString()) {
    appointments = newAppointments;
    sendAppointments();
  }
  setTimeout(calendarUpdate, 900000);
}
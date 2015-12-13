/*
  This app is run on the phone to periodically update the internal calendar,
  and push the results to the watch.
*/

var appointments = new Array(40).fill(0);

Pebble.addEventListener('ready', function(e) {
  console.log('JavaScript app ready and running!');
  setTimeout(calendarUpdate, 2000);
});

function retrieveAppointments() {
  var apiClientID = '1337';
  var apiClientSecret = 'LOL';
  var calendar = "eweitenberg@gmail.com";
  var timeMin = new Date();
  var timeMax = timeMin;
  timeMax.setHours(timeMax.getHours() + 12);
  var query = {
    "timeMin": timeMin.toISOString(),
    "timeMax": timeMax.toISOString(),
    "items": [
      { "id": calendar },
    ]
  };
  
  var req = new XMLHttpRequest();
  req.open('POST', "https://www.googleapis.com/calendar/v3/freeBusy?key=" + apiClientID, false);
  req.setRequestHeader('Content-Type', 'application/json');
  req.send(JSON.stringify(query));
  
  var response = JSON.parse(req.responseText);
  if (response.calendars[calendar].errors) {
    console.log("Errors from Google Calendar: " + 
                JSON.stringify(response.calendars[calendar].errors));
  }
  var freebusy = response.calendars[calendar].busy;
  if (!freebusy) {
    console.log("There was no free-busy information.");
  }
  
  var newAppointments = new Array(40).fill(0);
  for(var i = 0; i != 10; ++i) {
    newAppointments[4*i + 0] = Date.parse(freebusy[i].start).getHours();
    newAppointments[4*i + 1] = Date.parse(freebusy[i].start).getMinutes();
    newAppointments[4*i + 2] = Date.parse(freebusy[i].end).getHours();
    newAppointments[4*i + 3] = Date.parse(freebusy[i].end).getMinutes();
  }
}

function sendAppointments() { 
  Pebble.sendAppMessage( { '1': appointments },
    function(e) {
      console.log('Successfully delivered message with transactionId=' + e.data.transactionId);
    },
    function(e) {
      console.log('Unable to deliver message with transactionId=' + e.data.transactionId  + ' Error is: ' + e.error.message);
    }
  );
}

function calendarUpdate() {
  var newAppointments = retrieveAppointments();
  if (newAppointments != appointments) {
    appointments = newAppointments;
    sendAppointments();
  }
  setTimeout(calendarUpdate, 900000);
}
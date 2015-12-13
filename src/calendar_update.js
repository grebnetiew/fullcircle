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
  var query = '{' +
    '"timeMin": datetime, ' +
    '"timeMax": datetime, ' +
    '"items": [ ' + 
    '  { ' +
    '    "id": string ' +
    '  } ' +
    '] ' +
  '}';
  
  var req = new XMLHttpRequest();
  req.open('POST', "https://www.googleapis.com/calendar/v3/freeBusy");
  req.setRequestHeader('Content-Type', 'application/json');
  req.send(query);
  // Umm and now?
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
  retrieveAppointments();
  sendAppointments();
}
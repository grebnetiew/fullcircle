     // Your Client ID can be retrieved from your project in the Google
      // Developer Console, https://console.developers.google.com
      var CLIENT_ID = '1032312056474-qi5ttkn63d7g1p0gipqd3o6mgh8ln0da.apps.googleusercontent.com';
      var SCOPES = ["https://www.googleapis.com/auth/calendar.readonly"];
      /**
       * Check if current user has authorized this application.
       */
      function checkAuth() {
        gapi.auth.authorize(
          {
            'client_id': CLIENT_ID,
            'scope': SCOPES.join(' '),
            'immediate': true
          }, handleAuthResult);
      }
      /**
       * Handle response from authorization server.
       *
       * @param {Object} authResult Authorization result.
       */
      function handleAuthResult(authResult) {
        var authorizeDiv = document.getElementById('authorize-div');
        if (authResult && !authResult.error) {
          // Hide auth UI, then load client library.
          authorizeDiv.style.display = 'none';
          loadCalendarApi();
        } else {
          // Show auth UI, allowing the user to initiate authorization by
          // clicking authorize button.
          authorizeDiv.style.display = 'inline';
        }
      }
      /**
       * Initiate auth flow in response to user clicking authorize button.
       *
       * @param {Event} event Button click event.
       */
      function handleAuthClick(event) {
        gapi.auth.authorize(
          {client_id: CLIENT_ID, scope: SCOPES, immediate: false},
          handleAuthResult);
        return false;
      }
      /**
       * Load Google Calendar client library. List upcoming events
       * once client library is loaded.
       */
      function loadCalendarApi() {
        gapi.client.load('calendar', 'v3', listCalendarList);
      }
      /**
       * Print the summary and start datetime/date of the next ten events in
       * the authorized user's calendar. If no events are found an
       * appropriate message is printed.
       */
      function listCalendarList() {
        var request = gapi.client.calendar.calendarList.list();
        request.execute(function(resp) {
          for (var i = 0; i < resp.items.length; i++) {
            var calendarID = resp.items[i].id;
            var calendarSummary = resp.items[i].summary;
            appendOutput(calendarID, calendarSummary);
          }
        });
      }
      /**
       * Append a pre element to the body containing the given message
       * as its text node.
       *
       * @param {string} message Text to be placed in pre element.
       */
      function appendOutput(id, title) {
        var pre = document.getElementById('output');
        var textContent = document.createTextNode(title + '\n');
        pre.appendChild(textContent);
      }

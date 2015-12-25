     // Your Client ID can be retrieved from your project in the Google
      // Developer Console, https://console.developers.google.com
      /**
       * Check if current user has authorized this application.
       */
      function checkAuth() {
        gapi.auth.authorize(
          {
            'client_id': CLIENT_ID,
            'scope': SCOPES.join(' '),
            'immediate': true,
            'access_type': 'offline'
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
          {client_id: CLIENT_ID, scope: SCOPES, immediate: false, 'access_type': 'offline', 'approval_prompt': 'force'},
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
      
      var calendarCheckboxes = [];
      
      /**
       * Append a label element to the body containing the given calendar
       * as its node.
       */
      function appendOutput(id, title) {
        var output = document.getElementById('output');
        var newLabel = document.createElement('label');
        newLabel.class = "item";
        
        var textContent = document.createTextNode(title);
        newLabel.appendChild(textContent);
        
        var newInput = document.createElement('input');
        newInput.type = "checkbox";
        newInput.class = "item-checkbox";
        newInput.id = "calendar-" + id;
        newInput.value = id;
        calendarCheckboxes.push(newInput.id);
        newLabel.appendChild(newInput);
        
        output.appendChild(newLabel);
      }

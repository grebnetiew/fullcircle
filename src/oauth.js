// The main OAuth2 implementation for use in the Android app
// Using the configuration utility (and Google's code) we request
// a Refresh token for Offline use, which is then kept in local
// storage. We use this to obtain access tokens.

var oauth2_refresh_token = localStorage.getItem(1) || '';

// Prototype for an access token response
function oauth2_access_response(obj) {
  // Set defaults
  this.access_token = '';
  this.expires_in = 0;
  this.token_type = '';
  this.error = '';
  this.expires_on = new Date(); // now, so it's always expired
  // Take values from the object passed to us
  for (var prop in obj) {
    this[prop] = obj[prop];
  }
  // Calculate expiry date if appliccable
  if (this.expires_in) {
    this.expires_on = this.expires_on.setSeconds(this.expires_on.getSeconds() + this.expires_in);
  }
  this.is_valid = function() {
    if (!this.access_token || this.token_type != "Bearer") return false;
    if (this.expires_on && this.expires_on <= new Date()) return false;
    return true;
  };
}

var oauth2_access_token = oauth2_access_response();

// Uses the Refresh Token to get a new Access Token from Google endpoint.
// Reports if successful.
function OAuth2_getAccessToken() {
  if (!oauth2_refresh_token) {
    return false;
  }
  var query = {
    'client_id': oauth2_clientid,
    'client_secret': oauth2_clientsecret,
    'refresh_token': oauth2_refresh_token,
    'grant_type': 'refresh_token'
  };
  
  var req = new XMLHttpRequest();
  req.open('POST', "https://www.googleapis.com/oauth2/v4/token", false);
  req.setRequestHeader('Content-Type', 'application/json');
  req.send(JSON.stringify(query));

  var response = new oauth2_access_response(JSON.parse(req.responseText));
  
  if (response.is_valid()) {
    oauth2_access_token = response;
    return true;
  }
  // If this is reached, an error occurred. Try to report it.
  if (response.error) {
    console.log("OAuth2 get access token: Google returned error " + response.error);
  } else {
    console.log("OAuth2 get access token: Google returned garbage " + req.responseText);
  }
  return false;
}

function OAuth2_authorizedRequest(url, query) {
  if (!oauth2_access_token.is_valid() && !OAuth2_getAccessToken()) {
    return {"error": "Unable to obtain OAuth2 access token"};
  }
  var req = new XMLHttpRequest();
  req.open('POST', url, false);
  req.setRequestHeader('Content-Type', 'application/json');
  req.setRequestHeader('Authorization', 'Bearer ' + oauth2_access_token);
  req.send(JSON.stringify(query));
  return JSON.parse(req.responseText);
}

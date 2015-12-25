// This file assumes CLIENT_ID is set.

// Init function checks if we have a token, and shows appropriate interface if not
function gapiInitialize() {
}

// Runs if the user clicks the Authorize button.
// Gets an auth code, and exchanges it for a refresh token
function handleAuthClick(event) {
}

// Checks if access token is valid.
// Returns a boolean (true for valid).
function gapiIsValidToken(token) {
  var res = new XmlHttpRequest();
  res.open('GET', 'https://www.googleapis.com/oauth2/v3/tokeninfo?access_token=' + token, 'false');
  res.send();
  if(res.responseType != 'json' || res.response.error) {
    console.log("Checked a token and it was invalid, response was " + res.responseText);
    return false;
  }
  if(!res.response.audience || res.response.audience != CLIENT_ID) {
    console.log("Checked a token and it had mismatched audience, response was " + res.responseText);
    return false;
  }
  return true;
}

// Shows the permission popup and returns a refresh token
function gapiRequestPermission() {
}

// Exchanges a refresh token for an access token
function gapiRefreshToAccessToken(refreshToken) {
}

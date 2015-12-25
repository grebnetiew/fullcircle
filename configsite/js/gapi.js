// This file assumes CLIENT_ID is set.

// Init function checks if we have a token, and shows appropriate interface if not
function gapiInitialize() {
}

// Runs if the user clicks the Authorize button.
// Gets an auth code, and exchanges it for a refresh token
function handleAuthClick(event) {
}

// Checks if token is valid.
// Return 0 for invalid, 1 for access token, 2 for refresh token
function gapiIsValidToken(token) {
}

// Shows the permission popup and returns a refresh token
function gapiRequestPermission() {
}

// Exchanges a refresh token for an access token
function gapiRefreshToAccessToken(refreshToken) {
}

var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};


function locationSuccess(pos) {
  //creates a bool variable which will be sent to the C function to test if font needs to be made larger or smaller (for big words)
  var less_than_ten = 0;
  
  // create string variable to store the three words coming from the What3Words site in JSON formot (x.x.x)
    var words;
  
  // create the regular expression which will parse the string into individual words
//     var r = /[a-z]+/g;
      var r = /[a-z]+|[0-9]+/g;
  
  // create the array which will be used to store the individual words.
    var word_arr;
  
 // create the arrays which will store the left and right longitudes respectively
    var lattitude_arr;
    var longitude_arr;
  
  //create lattitude and longitude values
  var lattitude_num = pos.coords.latitude.toFixed(4);
  var longitude_num = pos.coords.longitude.toFixed(4);
  var lattitude =  lattitude_num.toString();
  var longitude = longitude_num.toString();

  lattitude_arr = lattitude.match(r);
  longitude_arr = longitude.match(r);
  
    console.log('Success requesting location!');
      var myAPIKey = '';
  // Construct URL
     var url = 'https://api.what3words.com/v2/reverse?coords=' + pos.coords.latitude + ',' + pos.coords.longitude + '&display=full&format=json&key=' + myAPIKey;


  // Send request to OpenWeatherMap
  xhrRequest(url, 'GET', 
    function(responseText) {
      // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);

      // get words and parse them
      words = json.words;
      word_arr = words.match(r);
      
//       word_arr[2] = "unenthusiastically";
      
      
// figure out if one of the three words is bigger than 10
      for(var i = 0; i<=2; i++) {
        if(word_arr[i].length <= 10) {
          console.log("less than or equal to 10!");
        }
        else if (word_arr[i].length > 10) {
          console.log("GREATER than 10!");
          less_than_ten = 1;
        }
      }
      
      //parse the lattitude and longitude so that it can be displayed on the C side of things (no floating point support for touple types apparently)
//       console.log('latitude = ' + pos.coords.latitude + ", longitude = " + pos.coords.longitude);
        console.log('latitude array = ' + lattitude_arr + ", longitude = " + longitude_arr);


  // Assemble dictionary using our keys
  var dictionary = {
    'WORD_ONE' : word_arr[0],
    'WORD_TWO' : word_arr[1],
    'WORD_THREE' : word_arr[2],
    "LAT_CHARACTERISTIC" : lattitude_arr[0],
    "LAT_MANTISSA" : lattitude_arr[1],
    "LON_CHARACTERISTIC" : longitude_arr[0], 
    "LON_MANTISSA" : longitude_arr[1],
    "LESS_THAN_TEN" : less_than_ten
  };
  
  
  // Send to Pebble
  Pebble.sendAppMessage(dictionary,
    function(e) {
      console.log('Words info sent to Pebble successfully!');
    },
    function(e) {
      console.log('Error sending words info to Pebble!');
    }
  );
      
      
      
    }      
  );

}

function locationError(err) {
  console.log('Error requesting location!');
}

function getWords() {
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    {timeout: 15000, maximumAge: 60000}
  );
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log('PebbleKit JS ready!');

    // Get the initial weather
    getWords();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received!');
    getWords();
  }                     
);





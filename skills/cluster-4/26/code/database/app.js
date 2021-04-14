// Modules
var level = require('level')
var express = require('express');
var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var fs = require('fs');
var url = require('url');


// Create or open the underlying LevelDB store
var db = level('./mydb', {valueEncoding: 'json'});

// Random number function -- this is a helper function to generate dummy data
function getRndInteger(min, max) {
    return Math.floor(Math.random() * (max - min + 1) ) + min;
}

var id = [];
var smoke = [];
var temp = [];
var time = [];
var data = [];
var lines = [];
var tabs= [];
var j = 0;
 fs.readFile('Smoke.txt','utf8',function(err,data){
  lines = data.split("\n");
  //console.log(lines[0]);
  for(var i = 1; i < lines.length; i++){
    tabs = lines[i].split('\t');

    //console.log(tabs[0]);
    time[j] = tabs[0];
    id[j] = tabs[1];
    smoke[j] = tabs[2];
    temp[j] = tabs[3];
    j++;
  }
  console.log(temp);
  console.log("line printed");
});



//console.log(r);
// function readFile(){
//   var line = r.split("\n");
//   console.log(line);
// }
var q;
// Points to index.html to serve webpage
app.get('/', function(req, res){
  res.sendFile(__dirname + '/index.html');
  q = url.parse(req.url, true).query;
});

// Function to stream from database
function readDB(arg) {
  db.createReadStream()
    .on('data', function (data) {
      //console.log(data.key, '=', data.value)
      // Parsed the data into a structure but don't have to ...
      var dataIn = {[data.key]: data.value};
      // Stream data to client
      io.emit('message', dataIn);
    })
    .on('error', function (err) {
      console.log('Oh my!', err)
    })
    .on('close', function () {
      console.log('Stream closed')
    })
    .on('end', function () {
      console.log('Stream ended')
    })
}

// When a new client connects
var clientConnected = 0; // this is just to ensure no new data is recorded during streaming
io.on('connection', function(socket){
  console.log('a user connected');
  clientConnected = 0;

  // Call function to stream database data
  readDB();
  clientConnected = 1;
  socket.on('disconnect', function(){
    console.log('user disconnected');
  });
});

// Listening on localhost:3000
http.listen(3000, function() {
  console.log('listening on *:3000');

});

var i = 0;

function intervalFunc() {
  if (clientConnected == 1) {

    // Get current time
    var date = time[i];
    //var tempPoints = temp[1];
    // Fill in data structure
    var value = [{id:id[i], temp: temp[i], smoke: smoke[i]}];

    i++;
    if(i >= 40){
      i = 0;
    }


    // Put structure into database based on key == date, and value
    db.put([date], value, function (err) {
      if (err) return console.log('Ooops!', err) // some kind of I/O error
    })

    // Parse data to send to client
    var msg = {[date]: value};
    // Send to client
    if(q.time == date){ //check if if the key is same as query string
      io.emit('message', msg);
    }


    // Log to console
    console.log(Object.keys(msg));
  }
}
// Do every 1500 ms
setInterval(intervalFunc, 1500);


// Every 15 seconds, write random information
// function putData(){
//   for(var i = 0; i < lines.length; i++){
//     var value = [{id:1, temp:temp[i]}];
//     var date = Date.now();
//     // console.log("Date: " + date + "\n");
//     // console.log("Value: " + value.toString() + "\n");
//
//     db.put([date], value, function (err) {
//       if (err) return console.log('Ooops!', err) // some kind of I/O error
//     })
//
//        var msg = {[date]: value};
//         // Send to client
//         io.emit('message', msg);
//
//         // Log to console
//         console.log(Object.keys(msg));
//
//   }
//
//
// }

// function intervalFunc() {
//   if (clientConnected == 1) {
//
//     // Get current time
//     var date = Date.now();
//
//     // Fill in data structure
// //    for() {}
//
//
//
//     var value = [{id: 1, temp: 100}];
//
//     // Put structure into database based on key == date, and value
//     db.put([date], value, function (err) {
//       if (err) return console.log('Ooops!', err) // some kind of I/O error
//     })
// }
//     // Parse data to send to client
//     var msg = {[date]: value};
//
//     // Send to client
//     io.emit('message', msg);
//
//     // Log to console
//     //console.log(Object.keys(msg));
//   }
// }
// Do every 1500 ms
//setInterval(putData, 500);

//setInterval(readFile,5000);

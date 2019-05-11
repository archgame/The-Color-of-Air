var mqtt = require('mqtt');
var http = require('http');

var client = mqtt.connect('mqtt://2ffedeff:4ca8d8c1141ec18c@broker.shiftr.io', {
  clientId: 'NI_javascript'
});

client.on('connect', function () {
    try{    //you can play around with this to see where it needs to go, should be fine here though
        console.log('client has connected!');
        client.subscribe('/OWM_Temperature');
        client.subscribe('/OWM_Humidity');
        client.subscribe('/Site');

        var parsedLat = 35.9622912;             //UTK CoAD Latitude
        var parsedLong = -83.9352017;           //UTK CoAD Longitutde

        var owmadd = "http://api.openweathermap.org/data/2.5/weather?lat=" + parsedLat + "&lon=" + parsedLong + "&APPID=4a357ba4ae201b67c40308da92492a22";
           
        setInterval(function () {
                
            http.get(owmadd, function (res) {
            
                var body = '';

                res.on('data', function (data) {
                    body += data;
                });

                res.on('end', function () {
                 
                 
                    var parsed = JSON.parse(body);
                    var parsedcoord = parsed.coord;
                    var parsedcoordlon = parsedcoord.lon;
                    var parsedcoordlat = parsedcoord.lat;
                    var parsedweather = parsed.weather;                       
                    var parsedmain = parsed.main;
                    var parsedmaintemp = parsedmain.temp;
                    var parsedmaintempC = parsedmaintemp - 273.15;
                    var parsedmaintempF = parsedmaintempC * 1.8 + 32;
                    var floatF = parsedmaintempF.toFixed(7);
                    var parsedsys = parsed.sys;
                    var parsedmainhumidity = parsedmain.humidity;
                    var parsedsyscountry = parsedsys.country;
                    var parsedname = parsed.name;
                    var time = new Date();   
                    
                    var output = 'CITY: ' + parsedname + ', COUNTRY: ' + parsedsyscountry + ', LONGITUDE: ' + parsedcoordlon + ', LATITUDE: ' + parsedcoordlat;
                        
                    var stringF = floatF.toString();
                    var stringH = parsedmainhumidity.toString();
                            
                    
                    client.publish('/OWM_Temperature', stringF);
                    client.publish('OWM_Humidity', stringH);
                    client.publish('/Site', output);
                });                            
            });
        }, 2000);     
    }
    catch (err)
    {
        //work here for what you want to do when the script stops working
        //you could leave this blank, I'm assuming it will keep cycling through the client.on function
        console.log('whoops disconnected!');
    }
});
    
client.on('message', function (topic, message)
{
  console.log('new message:', topic, message.toString());
});

//https://stackoverflow.com/questions/9339920/node-js-socket-exception-read-etimedout-how-do-i-catch-it-properly-what-about
client.on('error', function (err) {

    //https://github.com/nodejs/node-v0.x-archive/issues/6094
    if (err.code === 'ETIMEDOUT') {
        // ... do something useful here
        //maybe restart the code here
        console.log('We caught the ETIMEOUT error');
    }
    if (err.code === 'ENETDOWN') {
        // ... do something useful here
        //maybe restart the code here
        console.log('We caught the ENETDOWN error');
    }
    //id = socket2id.get(client); //i'm not sure what socket2id is, you might want to comment this and below out

    console.log('We caught the error');
    //removeSocketFromMap(id, client);
    //client.destroy();
});
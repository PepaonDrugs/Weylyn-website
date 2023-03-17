
var url = "https://api.minetools.eu/ping/weylyn.net";  
$.getJSON(url, function(r) {
  //data is the JSON string
  if (r.error) {
    $('#rest').html('Server Offline, Proxy or Server error occurred');
    return false;
  }
  var pl = '';
  if (r.players.sample.length > 0) {
    pl = '<br>OP: ' + r.players.sample[0].name;
  }
  $('#rest').html((/§(.+?)/gi, '') + '<br><b>Server Online</b> ');


});
var app = new Vue({
  el: '#app',
  data: {
    theApi: window.location.href,
    development: false,
    browsertime: Math.floor(Date.now() / 1000),
    debuginfo: [],
    devicetime: 0,
    logging: [],
    intervals: false,
    kitinfo: false,
    currentPage: 0,
    page: [
       {'visible': true,  'footer':'HOME',          'button': 'Start'},
       {'visible': false, 'footer':'REGISTER Key',  'button': 'Next'},
       {'visible': false, 'footer':'REGISTER WiFi', 'button': 'Next?'},
       {'visible': false, 'footer':'Connecting',    'button': 'Next?'},
       {'visible': false, 'footer':'Connecting',    'button': 'Next?'},
       {'visible': false, 'footer':'SD card',       'button': 'Next?'},
       {'visible': false, 'footer':'Debug info',    'button': 'Next?'},
       {'visible': false, 'footer':'Empty',         'button': 'End?'},
    ],
    publishinterval: 2,
    readinginterval: 60,
    selectedWifi: '',
    sensors: false,
    sensor1: false,
    sensor2: false,
    sensor3: false,
    sensor4: false,
    showAdvanced: false,
    showExperimental: false,
    showInterval: false,
    showSdCard: false,
    sdlog: false,
    usertoken: '',
    version: 'SCK 2.0 / SAM V0.0.2 / ESP V0.0.2',
    wifiname: '',
    wifipass: '',
    wifisync: true,
    wifis: {
      "nets": [{
        "ssid": "Fake-WIFI-1",
        "ch": 1,
        "rssi": -64
      }, {
        "ssid": "Fake-Wifi-2",
        "ch": 1,
        "rssi": -89
      }]
    }
  },
  mounted: function () {
    var el = document.getElementById('loading');
    el.parentNode.removeChild(el);
    // When the app is mounted
    this.selectApiUrl();
    var that = this;
    console.log('Vue.js mounted, fetching data at startup');

    setTimeout(function() {
      that.jsGet('aplist');
    }, 500);

    // This checks if connection to the kit has been lost, every 5 sec
    //this.every5sec();
  },
  methods: {
    every5sec: function () {
      var that = this;

      // TODO: should we check status every 5 sec?
      setTimeout(function(){
        return that.every5sec();
      }, 5000);
    },
    selectApiUrl: function () {
      // If we are running this from the kit,
      // the API should be on the same IP and port
      // Most likely a 192.168.*.1/status

      // If we are using port 8000, we are in development, and the API should be on port 3000
      if (window.location.port === '8000') {
        this.theApi = 'http://' + window.location.hostname + ':3000/';
        this.development = true;
      } else {
        this.theApi = window.location.href;
      }

      console.log('Using API : ' + this.theApi);
      this.notify('Using API', 1000);
    },
    httpGet: function(theUrl, callback) {
      var xmlHttp = new XMLHttpRequest();

      xmlHttp.onreadystatechange =  function() {
        if (xmlHttp.readyState == 4 && xmlHttp.status == 200) {
          callback(xmlHttp.responseText);
        }
      }
      xmlHttp.open( "GET", theUrl, true ); // false for synchronous request, true = async
      xmlHttp.send( null );
    },
    jsGet: function(path) {
      var that = this;

      this.httpGet(this.theApi + path, function(res){
        if (path === 'aplist') {
          that.wifis = JSON.parse(res);
          that.notify('Getting wifi list...', 1000, 'bg-cyan');
        }
        if (path === 'status'){
          that.notify('Getting status', 1000);
          that.debuginfo = JSON.parse(res);
        }

      });

    },
    // Not actually a POST request, but a GET with params
    jsPost: function(path, purpose){
      this.browsertime = Math.floor(Date.now() / 1000);
      var that = this;
      // Available parameters:
      // /set?ssid=value1&password=value2&token=value3&epoch=value

      if (purpose == 'connect'){
        this.notify('Connecting online...', 2000);
        that.httpGet(that.theApi + path +
            '?ssid=' + that.selectedWifi +
            '&password=' + that.wifipass +
            '&token=' + that.usertoken +
            '&epoch=' + that.browsertime,
            function(res){
              console.log('Kit response: ' + res);
            });
        // After we try to connect, we go to the next page.
        this.gotoPage();
      }

      if (purpose == 'synctime'){
        this.notify('Starting to log on SD CARD..', 2000);
        this.httpGet(this.theApi +  path + '?epoch=' + this.browsertime, function(res){
          // TODO: What is the correct response.key from the kit?
          that.notify(JSON.parse(res).todo, 5000);
        });
      }

    },

    gotoPage: function(num){

      // Find last page so we wont go to far, when clicking 'Next'
      if (!num && this.currentPage === (this.page.length - 1)) {
        console.log('Last page: ' + this.currentPage)
        return;
      }

      console.log(this.currentPage + ' / ' + this.page.length);

      // Hide current page
      this.page[this.currentPage].visible = false;

      // Find which page to show next
      if (num) {
        this.currentPage = num;
      }else{
        this.currentPage = parseInt(this.currentPage + 1);
      }

      // Show it
      this.page[this.currentPage].visible = true;

    },

    notify: function(msg, duration, className){

      //All events should also go to the logging section at the bottom in the advanced section
      this.logging.push(msg)

      var newtoast = document.createElement("div");
      if (className) {
        newtoast.className = "toast " + className;
      }else{
        newtoast.className = "toast";
      }
      newtoast.innerHTML = msg;
      document.getElementById("toast-wrapper").appendChild(newtoast);

      setTimeout(function(){
        newtoast.outerHTML = "";
        delete newtoast;
      }, duration);

      console.log('Notify:', msg)
    },
  },
  computed: {
    sortedWifi: function () {
      this.wifis.nets.sort(function (a, b) {
        return a.rssi - b.rssi;
      });
      return this.wifis.nets.reverse();
    },
    usertokenCheck: function(){
      return this.usertoken.length === 6;
    }
  }
});


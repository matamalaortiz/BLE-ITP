var POTEN_SERVICE = 'AAA0';
var STATUS_CHARACTERISTIC = 'AAA1';

var app = {
    initialize: function() {
        this.bindEvents();
        this.showMainPage();
    },
    bindEvents: function() {
        document.addEventListener('deviceready', this.onDeviceReady, false);
        document.addEventListener('backbutton', this.onBackButton, false);
        deviceList.addEventListener('click', this.connect, false);
        refreshButton.addEventListener('click', this.refreshDeviceList, false);
        disconnectButton.addEventListener('click', this.disconnect, false);
    },
    onDeviceReady: function() {
        FastClick.attach(document.body); // https://github.com/ftlabs/fastclick
        app.refreshDeviceList();
    },
    refreshDeviceList: function() {
        deviceList.innerHTML = ''; // empty the list
        ble.scan([POTEN_SERVICE], 5, app.onDiscoverDevice, app.onError);
    },
    onDiscoverDevice: function(device) {
        var listItem = document.createElement('li');
        listItem.innerHTML = device.name + '<br/>' +
            device.id + '<br/>' +
            'RSSI: ' + device.rssi;
        listItem.dataset.deviceId = device.id;
        deviceList.appendChild(listItem);
    },
    connect: function(e) {
        var deviceId = e.target.dataset.deviceId;
        ble.connect(deviceId, app.onConnect, app.onError);
    },
    onConnect: function(peripheral) {
        app.peripheral = peripheral;
        app.showDetailPage();

        var failure = function(reason) {
            navigator.notification.alert(reason, null, 'Button Error');
        };

        // subscribe to be notified when the button state changes
        ble.startNotification(
            peripheral.id,
            POTEN_SERVICE,
            STATUS_CHARACTERISTIC,
            app.onNotification,
            failure
        );

    },
    onNotification: function(buffer) {
        console.log('onNotification');
        var data = new Float32Array(buffer);
        var state = data[0];
        console.log(state);

        if ((state === 0 ) && (state < 200)) {
            document.body.style.backgroundColor = '#3600FF';
        } else if ((state > 201) && (state < 400)) {
            document.body.style.backgroundColor = "red";
        } else  if ((state > 401) && (state < 800)) {
          document.body.style.backgroundColor = "white";
          console.log('blanco');
        } else {
          document.body.style.backgroundColor = "black";
        }
    },
    disconnect: function(e) {
        if (app.peripheral && app.peripheral.id) {
            ble.disconnect(app.peripheral.id, app.showMainPage, app.onError);
        }
    },
    showMainPage: function() {
        mainPage.hidden = false;
        detailPage.hidden = true;
    },
    showDetailPage: function() {
        mainPage.hidden = true;
        detailPage.hidden = false;
    },
    onBackButton: function() {
        if (mainPage.hidden) {
            app.disconnect();
        } else {
            navigator.app.exitApp();
        }
    },
    onError: function(reason) {
        navigator.notification.alert(reason, app.showMainPage, 'Error');
    }
};

app.initialize();

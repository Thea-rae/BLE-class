var RELAY_SERVICE = 'FF01';
var SWITCH_CHARACTERISTIC;

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
        relay.addEventListener('change', this.relaySelected, false);
        onButton.addEventListener('click', this.switchOn, false);
        offButton.addEventListener('click', this.switchOff, false);
        disconnectButton.addEventListener('click', this.disconnect, false);
    },
    relaySelected: function(){
        var elem = (typeof this.selectedIndex === "undefined" ? window.event.srcElement :this);
        var value = elem.value || elem.options[elem.selectedIndex].value;
        switch (value){
            case '1':
                SWITCH_CHARACTERISTIC = 'FF02';
                break;
            case '2':
                SWITCH_CHARACTERISTIC = 'FF03';
                break;
            case '3':
                SWITCH_CHARACTERISTIC = 'FF04';
                break;
            case '4':
                SWITCH_CHARACTERISTIC = 'FF05';
                break;
        }
    },
    onDeviceReady: function() {
        FastClick.attach(document.body); // https://github.com/ftlabs/fastclick
        app.refreshDeviceList();
    },
    refreshDeviceList: function() {
        deviceList.innerHTML = ''; // empty the list
        ble.scan([RELAY_SERVICE], 5, app.onDiscoverDevice, app.onError);
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
    },
    disconnect: function(e) {
        if (app.peripheral && app.peripheral.id) {
            ble.disconnect(app.peripheral.id, app.showMainPage, app.onError);
        }
    },
    switchOn: function() {
        app.setSwitchValue(0);
    },
    switchOff: function() {
        app.setSwitchValue(1);
    },
    setSwitchValue: function(value) {
        console.log(SWITCH_CHARACTERISTIC);
        var success = function() {
            console.log('Set switch value to ' + value);
        };

        if (app.peripheral && app.peripheral.id) {
            var data = new Uint8Array(1);
            data[0] = value;
            ble.write(
                app.peripheral.id,
                RELAY_SERVICE,
                SWITCH_CHARACTERISTIC,
                data.buffer,
                success,
                app.onError
            );
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

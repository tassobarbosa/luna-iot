## Skeleton Code

```
System boot();

Check Communication();

Check Health();

Check Updates();

Wait authentication();

Task Manager() {

    define dinamyc priority for{
        Read Sensor:
            read Battery;    
            read Temperature;
            read Gyro;
            read Accel;
            read GPS;
            read Light Sensor;
            read RTC;

        Feedback Response:
            if sensor is low, hot, fast, dark:
                turn on RGB lights            

        Control Access Request:
            open Camera;
            control Bluetooth;
            control Wifi;        
        

        [Interruption] if some state change:
            push notification to cloud/mobile
    }

}

end();

```

## Code explanation and design


**Check Communication:** Checking the equipment's communication system is the first way to guarantee security that the entire cycle of use will be successful.
This system is extremely important because it is the one who will ensure that future sensor readings are transmitted to the user and to the central cloud.


**Check Health:** I am considering the health of the components to be paramount for the proper functioning of the equipment.
It is known that any malfunction in the system can lead to a tragedy, that's why the initial check of the physical system is so important, to guarantee the health of users.
In case of malfunction, the software will not allow the device to be used, an alert will be sent to the cloud central, and as a backup plan if the device is not connected to the internet, it will be requested to send the alert to the central via the user's cell phone application.


**Check Updates:** This system is not decisive for the operation of the equipment, however if it is identified that there is a firmware update, a download will be started in background. It will only be installed after next reboot.


**Authentication:** This is the feature that verifies the user's credentials. This execution phase is responsible for unlocking the scooter and sending initial alert messages to the user.


**Task Manager:** This is the most important part of the system when it comes to control. The Task Manager is responsible for scheduling the order of functionalities in dynamic and asynchronous order as needed.
It works axactly as an operating system. It should be able to understand which sensors are the most critical and what they are "saying", in order to generate an instant response to the user.
It is also responsible for responding to interruptions and saving energy when needed.

## State Diagram

![Alt text](state-diagram.png?raw=true "State Diagram")
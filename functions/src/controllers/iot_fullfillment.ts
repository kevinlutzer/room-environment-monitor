import {Request, Response} from 'express';
import {cloudiot_v1, google} from 'googleapis';

export const FullFillment = async (request: Request, response: Response) => {

    const auth = await google.auth.getClient({
        scopes: ['https://www.googleapis.com/auth/cloudiot']
      })

    const a = Buffer.from(`{fan_status:"ASDASD"}`).toString('base64');

    const client = new cloudiot_v1.Cloudiot({})
    client.projects.locations.registries.devices.modifyCloudToDeviceConfig({
        auth: auth,
        name: "projects/iot-klutzer/locations/us-central1/registries/devices-klutzer/devices/raspberry-pi-room-monitor-rs256-device",
        requestBody: {
            binaryData: a
        }
    }).then(data => console.log(data))

    response.send("Hello world");
}


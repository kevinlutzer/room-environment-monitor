import {Request, Response} from 'express';
import {cloudiot_v1, google} from 'googleapis';

async function getIOTClient(): Promise<cloudiot_v1.Cloudiot> {
    // Get auth client based on service account of google cloud functions
    const auth = await google.auth.getClient({
        scopes: ['https://www.googleapis.com/auth/cloudiot']
    })

    const client = new cloudiot_v1.Cloudiot({}) 
    client._options.auth = auth;
    return client;
}

async function setIOTDeviceConfig(client: cloudiot_v1.Cloudiot) {
    const a = Buffer.from(`{fan_status:"123ABC"}`).toString('base64');

    
    client.projects.locations.registries.devices.modifyCloudToDeviceConfig({
        name: "projects/iot-klutzer/locations/us-central1/registries/devices-klutzer/devices/raspberry-pi-room-monitor-rs256-device",
        requestBody: {
            binaryData: a
        }
    })
} 

export const FullFillment = async (request: Request, response: Response) => {
    setIOTDeviceConfig(await getIOTClient())
    response.send("Hello World");
}

export const Token = (request: Request, response: Response) => {
    
}
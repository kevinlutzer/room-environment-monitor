import {IOTPubsubMessageInterface} from '../model/iot-pubsub-message.interface';

export function ExtractInterfaceFromPubsubMessage(message: IOTPubsubMessageInterface): any {
    let rawData: any;
    try {
        rawData = JSON.parse(message.data ? Buffer.from(message.data, 'base64').toString() : null);
    } catch {
        console.error("Failed to parse the pubsub mesage. The api interface most likely changed");
        return null;
    }
    console.log("message: ", message, ", rawData: ", rawData);
    return rawData;
}
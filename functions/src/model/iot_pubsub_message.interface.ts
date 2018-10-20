import * as functions from 'firebase-functions';

export interface IOTPubsubMessageInterface extends functions.pubsub.Message{
    readonly attributes: {
        [key: string]: string;
        deviceId: string;
        deviceNumId: string;
        deviceRegistryId: string;
        deviceRegistryLocation: string;
        projectId: string;
        subFolder: string;
    };
}
const Datastore = require('@google-cloud/datastore');
const datastore = Datastore();
const kind = '';

export interface DataPoint {
    temperature: number,
    humidity: number,
    ambientLight: number
}

export interface RaspberryPiEnvironmentMonitoringPubSubMessage {
    timestamp: string,
    data: {  
       attributes: {  
          temperature: number,
          humidity: number,
          ambientLight: number
       }
    }
}



export function RaspberryPiEnvironmentMonitoringPubSubHandler(req: RaspberryPiEnvironmentMonitoringPubSubMessage, callback: Function) {

    let dateKey = (new Date(req.timestamp)).getTime().toString()
    const key = datastore.key([kind, dateKey])
    const entity = {
        key: key,
        data: {
          temperature: req.data && req.data.attributes ? req.data.attributes.temperature : null,
          humidity: req.data && req.data.attributes ? req.data.attributes.humidity : null,
          ambientLight: req.data && req.data.attributes ? req.data.attributes.ambientLight : null
        } as DataPoint
    };
    datastore.save(entity)
        .then((a: any) => console.log(a))
        .catch((err: Error) => {
            console.error(err);
            return Promise.reject(err);
        });

    callback();
}
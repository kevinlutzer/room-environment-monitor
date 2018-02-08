import { Request, Response } from 'express';
const Datastore = require('@google-cloud/datastore');
const datastore = Datastore();
const projectKind = 'Project';

export function projectLIST (req: Request, res: Response) {
    if ( req.method !== 'GET' ) {
        res.status(405).send(req.method + ' is not allowed');
    }

    const query = datastore
        .createQuery([projectKind])
        .start();

    datastore
        .runQuery(query, (err: Error, entities: any, nextQuery: any) => {
            if( err ) {
                res.status(500).send(err);
            }

            let hasMore = nextQuery.moreResults !== Datastore.NO_MORE_RESULTS ? nextQuery.endCursor : false;

            res.header('Access-Control-Allow-Origin', '*');
            res.header('Access-Control-Allow-Headers', 'Content-Type');
            res.status(200).send({
                projects: entities,
                hasMore: hasMore
            });
        });
}
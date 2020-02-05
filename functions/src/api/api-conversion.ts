import { Request, Response } from 'express';
import * as functions from 'firebase-functions';

export class ApiError {
    error: string;
    code: number;

    constructor(error: string, code: number) {
        this.error = error;
        this.code = code;
    }

    toApi(res: Response): void {
        res.json({
            error: this.error,
        });
        res.status(this.code);
    }
}

interface apiRequestInterface {
    pageSize: number; 
    cursor: number;  
    searchTerm: string;  
}

export class ApiListRequest {
    pageSize: number; 
    cursor: number;
    searchTerm: string;

    constructor(data: apiRequestInterface) {
        this.pageSize = data.pageSize;
        this.cursor = data.cursor || 0;
        this.searchTerm = data.searchTerm;
    }

    static fromRequest(r: Request): ApiListRequest {
        return new ApiListRequest({
            cursor: parseInt(r.query.cursor, 10) || 0,
            pageSize: parseInt(r.query.pageSize, 10) || 100,
            searchTerm: r.query.searchTerm || ''
        })
    }

}

export class ApiListResponse<T> {
    data: T[];
    nextCursor: number;
    hasMore: boolean;

    constructor(data: T[], r: ApiListRequest) {
        this.data = data;
        this.hasMore = r.pageSize === this.data.length;
        this.nextCursor = this.hasMore ? r.pageSize + r.cursor : 0
    }

    public toReponse(r: Response): void {
        r.json({
            data: this.data,
            hasMore: this.hasMore,
            nextCursor: this.nextCursor,
        })
    }
}

export function ExtractInterfaceFromPubsubMessage(message: functions.pubsub.Message): any {
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
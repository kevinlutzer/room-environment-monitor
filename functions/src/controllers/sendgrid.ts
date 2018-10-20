
import * as sendgrid from 'sendgrid';

import {SendGridApiKey} from '../config';
import {CreateSendgridPayload, SendgridPayloadInterface} from '../model/sendgrid.interface';

export async function SendSendgridEmail(recipients: string[], subject: string, message: string) {
    if (!message.length) {
        throw Error('The message must be greater the 1 character')
    }

    return Promise.resolve()
        .then(() => {
        const client = sendgrid(SendGridApiKey);

        // Build the SendGrid request to send email
        const request = client.emptyRequest({
            method: 'POST',
            path: '/v3/mail/send',
            body: CreateSendgridPayload(recipients, subject, message)
        });

        // Make the request to SendGrid's API
        console.log(`Sending email to: ${recipients}`);
        return client.API(request);
        }).catch( err => console.log(err.response.body.errors))
}

import * as sendgrid from 'sendgrid';

import {SendGridApiKey} from '../config';
import {Create} from '../model/sendgrid.interface';

export async function SendEmail(recipients: string[], subject: string, message: string) {
    if (!message.length) {
        throw Error('The message must be greater the 1 character, this is a sendgrid requirement')
    }

    return Promise.resolve()
        .then(() => {
            const client = sendgrid(SendGridApiKey);

            // Build the SendGrid request to send email
            const request = client.emptyRequest({
                method: 'POST',
                path: '/v3/mail/send',
                body: Create(recipients, subject, message)
            });

            // Make the request to SendGrid's API
            console.log(`Sending email to: ${recipients}`);
            return client.API(request);
        })
}
export interface SendgridPayloadInterface {
    personalizations: [{
        to: {email: string}[],
        subject: string
    }],
    from: {email: string},
    content: [{
        type: string,
        value: string
    }]
}

export function CreateSendgridPayload(reciepients: string[], subject: string, message: string): SendgridPayloadInterface {
    return {
        personalizations: [{
            to: reciepients.map(r => {return {email: r}}),
            subject: subject,
        }],
        from: {email: 'kevinlutzer9@gmail.com'},
        content: [{
            type: 'text/plain',
            value: message,
        }],
    }
}
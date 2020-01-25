
import { Response, Request, NextFunction } from 'express';

// Handlers
export async function SetConfig(req: Request, res: Response, next: NextFunction, db: FirebaseFirestore.Firestore) {
    console.log(req);
}

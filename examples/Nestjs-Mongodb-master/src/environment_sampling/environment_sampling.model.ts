import * as mongoose from 'mongoose';
import { Document } from 'mongoose';

// you have to set the collection neme here otherwise
// this is looking into environment_samplings

export const COLLECTION_NAME: string = 'environment_sampling';
export const environmentSamplingSchema = new mongoose.Schema(
  {
    time: { type: Date, required: true },
    acetone_concentration: { type: Number, required: false },
    carbon_monoxide_concentration: { type: Number, required: false },
    amonium_concentration: { type: Number, required: false },
    carbon_dixide_concentration: { type: Number, required: false },
    air_quality: { type: Number, required: false },
    temperature: { type: Number, required: false },
    humidity: { type: Number, required: false },
    toluene_concentration: { type: Number, required: false },
    alcohol_concentration: { type: Number, required: false },
  },
  { collection: COLLECTION_NAME },
);

export interface EnvironmentSampling extends Document {
  time: Date;
  acetone_concentration: number;
  carbon_monoxide_concentration: number;
  amonium_concentration: number;
  carbon_dixide_concentration: number;
  air_quality: number;
  temperature: number;
  humidity: number;
  toluene_concentration: number;
  alcohol_concentration: number;
}

import { Injectable } from '@nestjs/common';

import { InjectModel } from '@nestjs/mongoose';

import {
  COLLECTION_NAME,
  EnvironmentSampling,
} from './environment_sampling.model';
import { Model } from 'mongoose';

@Injectable()
export class EnvironmentSamplingService {
  constructor(
    @InjectModel(COLLECTION_NAME)
    private readonly environmentSamplingSchema: Model<EnvironmentSampling>,
  ) {}

  async getAllSamples(): Promise<EnvironmentSampling[]> {
    return this.environmentSamplingSchema.find().exec();
  }

  async getSample(envId: string): Promise<EnvironmentSampling> {
    return this.environmentSamplingSchema.findOne({ _id: envId }).exec();
  }

  async insertProduct() {
    const newProduct = new this.environmentSamplingSchema({
      time: Date(),
      acetone_concentration: 0.0,
      carbon_monoxide_concentration: 0.0,
      amonium_concentration: 0.0,
      carbon_dixide_concentration: 0.0,
      air_quality: 0.0,
      temperature: 0.0,
      humidity: 0.0,
      toluene_concentration: 0.0,
      alcohol_concentration: 0.0,
    });
    const result = await newProduct.save();
    console.log(result);
    return 'prodId';
  }
}

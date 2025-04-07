import { Module } from '@nestjs/common';
import { MongooseModule } from '@nestjs/mongoose';

import { EnvironmentSamplingController } from './environment_sampling.controller';
import { EnvironmentSamplingService } from './environment_sampling.service';
import {
  COLLECTION_NAME,
  environmentSamplingSchema,
} from './environment_sampling.model';

@Module({
  imports: [
    MongooseModule.forFeature([
      { name: COLLECTION_NAME, schema: environmentSamplingSchema },
    ]),
  ],
  controllers: [EnvironmentSamplingController],
  providers: [EnvironmentSamplingService],
})
export class EnvironmentSamplingModule {}

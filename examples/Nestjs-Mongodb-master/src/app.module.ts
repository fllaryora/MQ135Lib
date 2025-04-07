import { Module } from '@nestjs/common';
import { MongooseModule } from '@nestjs/mongoose';

import { AppController } from './app.controller';
import { AppService } from './app.service';
import { EnvironmentSamplingModule } from './environment_sampling/environment_sampling.module';

export const DATABASE_NAME: string = 'office';
@Module({
  imports: [
    EnvironmentSamplingModule,
    MongooseModule.forRoot(`mongodb://localhost:27017/${DATABASE_NAME}`),
  ],
  controllers: [AppController],
  providers: [AppService],
})
export class AppModule {}

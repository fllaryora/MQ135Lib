import { NestFactory } from '@nestjs/core';
import { AppModule } from './app.module';
import { Logger } from '@nestjs/common';

//https://docs.nestjs.com/security/cors
// https://agnesmuita.medium.com/cors-error-solved-access-to-fetch-has-been-blocked-by-cors-policy-9ac753cafe5a
//https://developer.mozilla.org/en-US/docs/Web/HTTP/Guides/CORS#preflighted_requests
//https://github.com/expressjs/cors#configuration-options
async function bootstrap() {
  const app = await NestFactory.create(AppModule);
  app.enableCors({
    origin: 'null', //'http://localhost:3333', // default *
    methods: 'GET,HEAD,PUT,PATCH,POST,DELETE',
    credentials: true, // default undefined
    preflightContinue: false,
    optionsSuccessStatus: 204,
  });
  await app.listen(3333);
  const logger = new Logger('bootstrap');
  logger.log(`Listening on ${await app.getUrl()}`);
}
bootstrap();

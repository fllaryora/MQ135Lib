import { Controller, Post, Get, Param, Header, Options } from '@nestjs/common';

import { EnvironmentSamplingService } from './environment_sampling.service';

@Controller('environment_sampling')
export class EnvironmentSamplingController {
  constructor(
    private readonly environmentSamplingService: EnvironmentSamplingService,
  ) {}

  @Get()
  getAllSamples() {
    return this.environmentSamplingService.getAllSamples();
  }

  @Get(':id')
  getSample(@Param('id') sampleId: string) {
    return this.environmentSamplingService.getSample(sampleId);
  }

  @Post()
  addProduct() {
    const generatedId = this.environmentSamplingService.insertProduct();
    return { id: generatedId };
  }
}

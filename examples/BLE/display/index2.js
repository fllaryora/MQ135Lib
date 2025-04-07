async function fetchData(){

    try{

        const response = await fetch("http://localhost:3333/environment_sampling", {
            mode: 'cors',
            credentials: 'include',
            method: "get",
            headers: {
                 "Content-Type": "application/json",
            }
        });

        if(!response.ok){
            console.error(response);
            throw new Error("Could not fetch resource");
        }
        const data = await response.json();
        
        const sample = data[0];
        //{"time":"2025-04-05T19:06:22.000Z",
        // "alcohol_concentration":0,
        // "__v":0,
        // "carbon_monoxide_concentration":0,
        // "air_quality":0,
        // "carbon_dixide_concentration":0,
        // "temperature":0,
        // "_id":"67f17f2ea7912e2f7d0a72a8",
        // "humidity":0,
        // "amonium_concentration":0,
        // "toluene_concentration":0,
        // "acetone_concentration":0}

        const retrievedTemperatureValue = document.getElementById('temperatureContainer');
        const retrievedHumidityValue = document.getElementById('humidityContainer');
        const retrievedAirQualityValue = document.getElementById('airQualityContainer');
        const retrievedCoValue = document.getElementById('co2Container');
        const retrievedCo2Value = document.getElementById('coContainer');
        const timestampContainer = document.getElementById('timestamp');

        retrievedTemperatureValue.innerHTML = sample.temperature;
        retrievedHumidityValue.innerHTML = sample.humidity;
        retrievedAirQualityValue.innerHTML = sample.air_quality;
        retrievedCoValue.innerHTML = sample.carbon_monoxide_concentration;
        retrievedCo2Value.innerHTML = sample.carbon_dixide_concentration;
        timestampContainer.innerHTML = sample.time;


        function getTime(sample) {
          return sample.time;
        }

        function getTemp(sample) {
          return sample.temperature;
        }

        function getHumidity(sample) {
          return sample.humidity;
        }

        function getAirQuality(sample) {
          return sample.air_quality;
        }

        function getCo(sample) {
          return sample.carbon_monoxide_concentration;
        }

        function getCo2(sample) {
          return sample.carbon_dixide_concentration;
        }

        const xValues = data.map(getTime);
        const yValueTemp = data.map(getTemp);
        const yValuesHumidity = data.map(getHumidity);
        const yValuesAirQuality = data.map(getAirQuality);

        const yValuesCo = data.map(getCo);
        const yValuesCo2 = data.map(getCo2);

        new Chart("temperatureChart", {
          type: "line",
          data: {
            labels: xValues,
            datasets: [{ 
              data: yValueTemp,
              borderColor: "red",
              fill: true
            }]
          },
          options: {
            legend: {display: false}
          }
        });

        new Chart("humidityChart", {
          type: "line",
          data: {
            labels: xValues,
            datasets: [{ 
              data: yValuesHumidity,
              borderColor: "green",
              fill: false
            },]
          },
          options: {
            legend: {display: false}
          }
        });

        new Chart("airChart", {
          type: "line",
          data: {
            labels: xValues,
            datasets: [{ 
              data: yValuesAirQuality,
              borderColor: "blue",
              fill: false
            }]
          },
          options: {
            legend: {display: false}
          }
        });

        new Chart("ppmChart", {
          type: "line",
          data: {
            labels: xValues,
            datasets: [{ 
              data: yValuesCo,
              borderColor: "red",
              fill: false
            }, { 
              data: yValuesCo2,
              borderColor: "green",
              fill: false
            }]
          },
          options: {
            legend: {display: false}
          }
        });

    }
    catch(error){
        console.error(error);
    }
}


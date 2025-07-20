// Wait for DOM to be fully loaded
document.addEventListener('DOMContentLoaded', function() {
  // Example ECU data structure for field selection
  const exampleECUData = {
    rpm: 0,
    TPS: 0,
    CLT: 0,
    IAT: 0,
    PW1: 0,
    MAP: 0,
    AFR1: 0
  };

  // Default configuration
  const defaultConfig = {
    gauges: [
      { id: 'gauge1', field: 'rpm', title: 'RPM', min: 0, max: 9000, units: 'RPM' },
      { id: 'gauge2', field: 'TPS', title: 'TPS', min: 0, max: 100, units: '%' },
      { id: 'gauge3', field: 'CLT', title: 'CLT', min: -40, max: 180, units: '°C' },
      { id: 'gauge4', field: 'IAT', title: 'IAT', min: -40, max: 120, units: '°C' },
      { id: 'gauge5', field: 'PW1', title: 'PW', min: 0, max: 20, units: 'ms' },
      { id: 'gauge6', field: 'MAP', title: 'MAP', min: 0, max: 250, units: 'kPa' },
      { id: 'gauge7', field: 'AFR1', title: 'AFR', min: 10, max: 20, units: 'λ' }
    ]
  };

  // Load configuration from localStorage
  function loadConfiguration() {
    try {
      const savedConfig = localStorage.getItem('gaugeDashboardConfig');
      return savedConfig ? JSON.parse(savedConfig) : defaultConfig;
    } catch (e) {
      console.error('Error loading config:', e);
      return defaultConfig;
    }
  }

  // Save configuration to localStorage
  function saveConfiguration(config) {
    try {
      localStorage.setItem('gaugeDashboardConfig', JSON.stringify(config));
      applyConfiguration(config);
    } catch (e) {
      console.error('Error saving config:', e);
    }
  }

  // Apply configuration to gauges
  function applyConfiguration(config) {
    if (!config || !config.gauges) return;

    config.gauges.forEach(gaugeConfig => {
      // Update gauge display values if elements exist
      const valueElement = document.getElementById(`${gaugeConfig.id}-value`);
      const labelElement = document.getElementById(`${gaugeConfig.id}-label`);
      
      if (valueElement) {
        valueElement.textContent = '0'; // Reset value
      }
      
      if (labelElement) {
        labelElement.textContent = gaugeConfig.title;
      }

      // Update gauge configuration if gauge exists
      const gauge = window.gauges?.find(g => g.options?.renderTo === gaugeConfig.id);
      if (gauge) {
        gauge.update({
          title: gaugeConfig.title,
          units: gaugeConfig.units,
          minValue: gaugeConfig.min,
          maxValue: gaugeConfig.max
        });
      }
    });
  }

  // Initialize designer mode
  function initDesignerMode() {
    const config = loadConfiguration();
    
    // Create designer button if it doesn't exist
    if (!document.getElementById('designer-button')) {
      const designerBtn = document.createElement('button');
      designerBtn.id = 'designer-button';
      designerBtn.className = 'floating-button';
      designerBtn.innerHTML = '<svg viewBox="0 0 24 24"><path d="M20.71 7.04c.39-.39.39-1.04 0-1.41l-2.34-2.34c-.37-.39-1.02-.39-1.41 0l-1.84 1.83 3.75 3.75M3 17.25V21h3.75L17.81 9.93l-3.75-3.75L3 17.25z"/></svg>';
      document.body.appendChild(designerBtn);
    }

    // Create modal if it doesn't exist
    if (!document.getElementById('designer-modal')) {
      const modal = document.createElement('div');
      modal.id = 'designer-modal';
      modal.className = 'modal';
      modal.style.display = 'none';
      modal.innerHTML = `
        <div class="modal-content">
          <h2>Gauge Designer</h2>
          <div id="gauge-config-forms"></div>
          <div class="modal-buttons">
            <button id="save-config">Save Configuration</button>
            <button id="cancel-design">Cancel</button>
          </div>
        </div>
      `;
      document.body.appendChild(modal);
    }

    // Populate forms
    function populateForms() {
      const formsContainer = document.getElementById('gauge-config-forms');
      if (!formsContainer) return;
      
      formsContainer.innerHTML = '';
      
      config.gauges.forEach((gauge, index) => {
        const form = document.createElement('div');
        form.className = 'config-form';
        form.innerHTML = `
          <h3>${gauge.id}</h3>
          <label>Title: <input type="text" data-index="${index}" data-field="title" value="${gauge.title || ''}"></label>
          <label>Data Field: 
            <select data-index="${index}" data-field="field">
              ${Object.keys(exampleECUData).map(key => 
                `<option value="${key}" ${key === gauge.field ? 'selected' : ''}>${key}</option>`
              ).join('')}
            </select>
          </label>
          <label>Min: <input type="number" data-index="${index}" data-field="min" value="${gauge.min || 0}"></label>
          <label>Max: <input type="number" data-index="${index}" data-field="max" value="${gauge.max || 100}"></label>
          <label>Units: <input type="text" data-index="${index}" data-field="units" value="${gauge.units || ''}"></label>
        `;
        formsContainer.appendChild(form);
      });
    }

    // Event listeners
    document.getElementById('designer-button')?.addEventListener('click', () => {
      populateForms();
      document.getElementById('designer-modal').style.display = 'flex';
    });

    document.getElementById('cancel-design')?.addEventListener('click', () => {
      document.getElementById('designer-modal').style.display = 'none';
    });

    document.getElementById('save-config')?.addEventListener('click', () => {
      const inputs = document.querySelectorAll('#gauge-config-forms input, #gauge-config-forms select');
      inputs.forEach(input => {
        const index = input.dataset.index;
        const field = input.dataset.field;
        if (index !== undefined && field && config.gauges[index]) {
          if (input.type === 'number') {
            config.gauges[index][field] = parseFloat(input.value) || 0;
          } else {
            config.gauges[index][field] = input.value;
          }
        }
      });
      saveConfiguration(config);
      document.getElementById('designer-modal').style.display = 'none';
    });
  }

  // Initialize when gauges are ready
  function checkGaugesReady() {
    if (window.gauges && window.gauges.length > 0) {
      initDesignerMode();
      applyConfiguration(loadConfiguration());
    } else {
      setTimeout(checkGaugesReady, 100);
    }
  }

  checkGaugesReady();

  window.updateGaugesWithConfiguration = function(ecuDataFrame) {
    const config = loadConfiguration();
    
    config.gauges.forEach(gaugeConfig => {
      if (ecuDataFrame[gaugeConfig.field] !== undefined) {
        const gauge = window.gauges?.find(g => g.options?.renderTo === gaugeConfig.id);
        if (gauge) {
          gauge.value = ecuDataFrame[gaugeConfig.field];
          gauge.update();
        }
        
        const valueElement = document.getElementById(`${gaugeConfig.id}-value`);
        if (valueElement) {
          valueElement.textContent = gaugeConfig.field === 'AFR1' 
            ? ecuDataFrame[gaugeConfig.field].toFixed(1)
            : Math.round(ecuDataFrame[gaugeConfig.field]);
        }
      }
    });
  };

});
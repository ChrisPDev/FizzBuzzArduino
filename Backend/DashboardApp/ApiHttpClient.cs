using Microsoft.JSInterop;
using System.Net.Http.Headers;

namespace DashboardApp
{
    public class ApiHttpClient
    {
        private readonly HttpClient _httpClient;
        private readonly IJSRuntime _jsRuntime;

        public ApiHttpClient(HttpClient httpClient, IJSRuntime jsRuntime)
        {
            _httpClient = httpClient;
            _jsRuntime = jsRuntime;
        }

        public async Task<HttpResponseMessage> GetAsync(string requestUri)
        {
            var token = await _jsRuntime.InvokeAsync<string>("localStorage.getItem", "authToken");
            if (!string.IsNullOrEmpty(token))
            {
                _httpClient.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Bearer", token);
            }

            return await _httpClient.GetAsync(requestUri);
        }

        // Implement similar methods for POST, PUT, DELETE etc.
    }

}

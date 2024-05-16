namespace ArduinoAPI.Models
{
    public class User : Common
    {
        public string username { get; set; }
        public string hashPass { get; set; }
        public string salt { get; set; }
    }
}

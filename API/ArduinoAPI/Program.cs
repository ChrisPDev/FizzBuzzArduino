using ArduinoAPI.Data;
using Microsoft.EntityFrameworkCore;
using System.Text;

namespace ArduinoAPI
{
    public class Program
    {
        // Try to revert a commit
        int i = 0;
        public static void Main(string[] args)
        {
            var builder = WebApplication.CreateBuilder(args);

            // Add services to the container.
            builder.Services.AddControllers();
            builder.Services.AddEndpointsApiExplorer();
            builder.Services.AddSwaggerGen();

            // Configure Entity Framework with PostgreSQL
            builder.Services.AddDbContext<ApplicationDbContext>(options =>
                options.UseNpgsql(builder.Configuration.GetConnectionString("DefaultConnection")));

            var app = builder.Build();

            // Configure the HTTP request pipeline.
            app.UseSwagger();
            app.UseSwaggerUI();

            app.UseHttpsRedirection();

            //app.UseAuthentication(); // Add this line to enable authentication
            //app.UseAuthorization();

            app.MapControllers();

            app.Run();
        }
    }
}

#include <iostream>
#include <fstream>
#include <libusb.h>
#include <string>
#include <sstream>
#include <vector>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

void printdev(libusb_device *dev, json& jsonArray); // Prototype

struct DeviceInfo
{
    string manufacturer;
    string product;
    uint16_t vendorId;
    uint16_t productId;
};

int main()
{
    libusb_device **devs;
    libusb_context *ctx = NULL;
    int r;
    ssize_t cnt;
    r = libusb_init(&ctx);
    if (r < 0)
    {
        printf("Init Error %d\n", r);
        return 1;
    }
    libusb_set_debug(ctx, 3);
    cnt = libusb_get_device_list(ctx, &devs);
    if (cnt < 0)
    {
        printf("Get Device Error\n");
    }

    json devicesJson = json::array();

    for (int i = 0; i < cnt; i++)
    {
        printdev(devs[i], devicesJson);
    }
    libusb_free_device_list(devs, 1);
    libusb_exit(ctx);

    cout << devicesJson << endl;  // print the complete JSON array

    return 0;
}

void printdev(libusb_device *dev, json& jsonArray)
{
    struct libusb_device_descriptor desc;
    int r = libusb_get_device_descriptor(dev, &desc);
    if (r < 0)
    {
        cout << "failed to get device descriptor" << endl;
        return;
    }

    libusb_device_handle *handle = NULL;
    char string[256];
    r = libusb_open(dev, &handle);

    DeviceInfo deviceInfo;
    deviceInfo.vendorId = desc.idVendor;
    deviceInfo.productId = desc.idProduct;

    if (LIBUSB_SUCCESS == r)
    {
        if (desc.iManufacturer)
        {
            r = libusb_get_string_descriptor_ascii(handle, desc.iManufacturer, (unsigned char *)string, sizeof(string));
            if (r > 0)
                deviceInfo.manufacturer = string;
        }

        if (desc.iProduct)
        {
            r = libusb_get_string_descriptor_ascii(handle, desc.iProduct, (unsigned char *)string, sizeof(string));
            if (r > 0)
                deviceInfo.product = string;
        }
        libusb_close(handle);
    }

    json j;

    j["manufacturer"] = deviceInfo.manufacturer;
    j["product"] = deviceInfo.product;
    j["vendorId"] = deviceInfo.vendorId;
    j["productId"] = deviceInfo.productId;

    jsonArray.push_back(j);  // add the JSON object to the array
}

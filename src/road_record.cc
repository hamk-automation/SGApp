#include "libs.h"
#include "road_record.h"
#include "utils.h"

RoadRecord::RoadRecord()
  : m_data(NULL)
{
}

RoadRecord::~RoadRecord()
{
  delete[] m_data;
}

bool RoadRecord::SaveToFile(const char* filename)
{
  std::ofstream file_out;
  file_out.open(filename, std::ios_base::binary);

  if (!file_out.is_open()) {
    return false;
  }

  // Write the header
  file_out.write((const char*)this, sizeof(RoadRecord) - sizeof(ScanDataPack*));

  // Write the data
  for (uint32_t i=0; i<m_num_packs; ++i) {
    char* data_buffer;
    int data_size = m_data[i].SaveToNewBuffer(&data_buffer);
    file_out.write(data_buffer, data_size);
    delete[] data_buffer;
  }

  file_out.close();

  return true;
}

bool RoadRecord::ReadFromFile(const char* filename)
{
  std::ifstream fin;

  fin.open(filename, std::ios_base::binary);
  if (!fin.is_open()) {
    return false;
  }

  // Read the header
  fin.read((char*)this, sizeof(RoadRecord) - sizeof(ScanDataPack*));

  // Read the data
  m_data = new ScanDataPack[m_num_packs];
//  fin.seekg(sizeof(RoadRecord) - sizeof(ScanDataPack*), std::ios::beg);
  fin.seekg(28, std::ios::beg); // For compatibility with Raspbian Pi system
  for (uint32_t i=0; i<m_num_packs; ++i) {
    uint32_t num_points;
    fin.read((char*)&num_points, sizeof(uint32_t));

    TimeStamp time_stamp;
    fin.read((char*)&time_stamp, sizeof(TimeStamp));

    float vel;
    fin.read((char*)&vel, sizeof(float));

    YawPitchRoll angle;
    fin.read((char*)&angle, sizeof(YawPitchRoll));

    uint16_t* ranges = new uint16_t[num_points];
    fin.read((char*)ranges, sizeof(uint16_t)*num_points);

    m_data[i].SetNumPoints(num_points);
    m_data[i].SetTimeStamp(time_stamp);
    m_data[i].SetVelocity(vel);
    m_data[i].SetRotationAngle(angle);
    m_data[i].SetRangeList(ranges);
  }

  fin.close();

  return true;
}

void RoadRecord::Filter()
{
  double* vel = new double[m_num_packs];
  double* filtered_vel = new double[m_num_packs];
  double* yaw = new double[m_num_packs];
  double* pitch = new double[m_num_packs];
  double* roll = new double[m_num_packs];
  double* filtered_yaw = new double[m_num_packs];
  double* filtered_pitch = new double[m_num_packs];
  double* filtered_roll = new double[m_num_packs];
  for (uint32_t i=0; i<m_num_packs; ++i) {
    vel[i] = m_data[i].GetVelocity();
    yaw[i] = m_data[i].GetRotationAngles().yaw;
    pitch[i] = m_data[i].GetRotationAngles().pitch;
    roll[i] = m_data[i].GetRotationAngles().roll;
  }
  Utils::FilterData(m_num_packs, vel, &filtered_vel);
  Utils::FilterData(m_num_packs, yaw, &filtered_yaw);
  Utils::FilterData(m_num_packs, pitch, &filtered_pitch);
  Utils::FilterData(m_num_packs, roll, &filtered_roll);
  for (uint32_t i=0; i<m_num_packs; ++i) {
    YawPitchRoll filtered_angles;
    filtered_angles.yaw = filtered_yaw[i];
    filtered_angles.pitch = filtered_pitch[i];
    filtered_angles.roll = filtered_roll[i];
    m_data[i].SetVelocity(filtered_vel[i]);
    m_data[i].SetRotationAngle(filtered_angles);
  }
  delete[] vel;
  delete[] yaw;
  delete[] pitch;
  delete[] roll;
  delete[] filtered_vel;
  delete[] filtered_yaw;
  delete[] filtered_pitch;
  delete[] filtered_roll;

//  uint32_t num_points = m_data[0].GetNumPoints();
//  uint16_t** new_ranges = new uint16_t*[m_num_packs];
//  for (uint32_t i=0; i<m_num_packs; ++i) {
//    new_ranges[i] = new uint16_t[num_points];
//  }
//  for (uint32_t j=0; j<num_points; ++j) {
//    double* ranges = new double[m_num_packs];
//    double* filtered_ranges = new double[m_num_packs];
//    for (uint32_t i=0; i<m_num_packs; ++i) {
//      ranges[i] = m_data[i].GetRangeList()[j];
//    }
//    Utils::FilterData(m_num_packs, ranges, &filtered_ranges);
//    if (j==250) {
//      for (uint32_t i=0; i<m_num_packs; ++i) {
////        DEBUG_PRINT("%lf, %lf;", ranges[i], filtered_ranges[i]);
//      }
//    }
//    for (uint32_t i=0; i<m_num_packs; ++i) {
//      new_ranges[i][j] = filtered_ranges[i];
//    }
//    delete[] ranges;
//    delete[] filtered_ranges;
//  }
//  for (uint32_t i=0; i<m_num_packs; ++i) {
//    uint16_t* old_ranges = m_data[i].GetRangeList();
//    delete[] old_ranges;
//    m_data[i].SetRangeList(new_ranges[i]);
//  }
}

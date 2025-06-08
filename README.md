## 🎬 Film Shot Type Classification

This project focuses on the automatic classification of cinematic shot types (such as **close-up**, **medium**, and **wide** shots) from video or image data. The classification is based on detecting and analyzing visual features — primarily faces and objects — using classical computer vision techniques (e.g., **Haar cascades**) and handcrafted feature extraction.

The motivation for this tool is to support tasks such as:
- **Film style analysis**
- **Shot distribution statistics**
- **Automated metadata generation** for video datasets

The application processes input frame-by-frame, extracts spatial and geometric features, classifies the shot type, and optionally evaluates the performance against a labeled dataset.

---

## 📂 Source Files (`src/`)

The following source files implement the full data flow of the system:

- `main` – Program entry point  
- `FeatureDetector` – Object detection using Haar cascades  
- `FeatureProcessorAndClassifier` – Feature extraction and shot type classification logic  
- `FileLoader` – Abstract interface for loading images or video  
- `FilmStatisticEval` – Aggregates classification results across frames  
- `ResultDisplayer` – Displays or exports results  
- `TestDatasetEval` – Compares predictions with ground truth labels  
- `UserStructs` – Definitions of shared data types and enums
